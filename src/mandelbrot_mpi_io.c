// Felipe Brigalante    8941280
// Mateus Rocha Mazzari 8941255

#include "mpi.h"
#include "omp.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define MASTER 0
#define CHUNK 100

double c_x_min;
double c_x_max;
double c_y_min;
double c_y_max;

double pixel_width;
double pixel_height;

int iteration_max = 200;

int image_size;
unsigned char **image_buffer;

int i_x_max;
int i_y_max;
int image_buffer_size;

int gradient_size = 16;
int colors[17][3] = {
    {66, 30, 15},    {25, 7, 26},     {9, 1, 47},      {4, 4, 73},
    {0, 7, 100},     {12, 44, 138},   {24, 82, 177},   {57, 125, 209},
    {134, 181, 229}, {211, 236, 248}, {241, 233, 191}, {248, 201, 95},
    {255, 170, 0},   {204, 128, 0},   {153, 87, 0},    {106, 52, 3},
    {16, 16, 16},
};

void allocate_image_buffer() {
  int rgb_size = 3;
  image_buffer =
      (unsigned char **)malloc(sizeof(unsigned char *) * image_buffer_size);

  for (int i = 0; i < image_buffer_size; i++) {
    image_buffer[i] = (unsigned char *)malloc(sizeof(unsigned char) * rgb_size);
  };
};

void init(int argc, char *argv[]) {
  if (argc < 6) {
    printf(
        "usage: ./mandelbrot_omp c_x_min c_x_max c_y_min c_y_max image_size\n");
    printf("examples with image_size = 11500:\n");
    printf(
        "    Full Picture:         ./mandelbrot_omp -2.5 1.5 -2.0 2.0 11500\n");
    printf("    Seahorse Valley:      ./mandelbrot_omp -0.8 -0.7 0.05 0.15 "
           "11500\n");
    printf("    Elephant Valley:      ./mandelbrot_omp 0.175 0.375 -0.1 0.1 "
           "11500\n");
    printf("    Triple Spiral Valley: ./mandelbrot_omp -0.188 -0.012 0.554 "
           "0.754 11500\n");
    exit(0);
  } else {
    sscanf(argv[1], "%lf", &c_x_min);
    sscanf(argv[2], "%lf", &c_x_max);
    sscanf(argv[3], "%lf", &c_y_min);
    sscanf(argv[4], "%lf", &c_y_max);
    sscanf(argv[5], "%d", &image_size);

    i_x_max = image_size;
    i_y_max = image_size;
    image_buffer_size = image_size * image_size;

    pixel_width = (c_x_max - c_x_min) / i_x_max;
    pixel_height = (c_y_max - c_y_min) / i_y_max;
  };
};

void update_rgb_buffer(int iteration, int x, int y) {
  int color;

  if (iteration == iteration_max) {
    image_buffer[(i_y_max * y) + x][0] = colors[gradient_size][0];
    image_buffer[(i_y_max * y) + x][1] = colors[gradient_size][1];
    image_buffer[(i_y_max * y) + x][2] = colors[gradient_size][2];
  } else {
    color = iteration % gradient_size;

    image_buffer[(i_y_max * y) + x][0] = colors[color][0];
    image_buffer[(i_y_max * y) + x][1] = colors[color][1];
    image_buffer[(i_y_max * y) + x][2] = colors[color][2];
  };
};

void write_to_file() {
  FILE *file;
  char *filename = "output_ompi.ppm";
  char *comment = "# ";

  int max_color_component_value = 255;

  file = fopen(filename, "wb");

  fprintf(file, "P6\n %s\n %d\n %d\n %d\n", comment, i_x_max, i_y_max,
          max_color_component_value);

  for (int i = 0; i < image_buffer_size; i++) {
    fwrite(image_buffer[i], 1, 3, file);
  };

  fclose(file);
};

void compute_mandelbrot_kernel(int *local_iteration_buffer, int init_k,
                               int chunksize) {
  double z_x;
  double z_y;
  double z_x_squared;
  double z_y_squared;
  double escape_radius_squared = 4;

  int iteration;
  int i_x;
  int i_y;

  int i;

  double c_x;
  double c_y;
  int i_k = init_k;

  for (i = 0; i < chunksize; i++) {
    i_k = init_k + i;
    if (i_k < i_y_max * i_x_max) {

      i_y = i_k / i_x_max;
      c_y = c_y_min + i_y * pixel_height;
      if (fabs(c_y) < pixel_height / 2) {
        c_y = 0.0;
      }

      i_x = i_k % i_x_max;
      c_x = c_x_min + i_x * pixel_width;

      z_x = 0.0;
      z_y = 0.0;

      z_x_squared = 0.0;
      z_y_squared = 0.0;

      for (iteration = 0; iteration < iteration_max &&
                          ((z_x_squared + z_y_squared) < escape_radius_squared);
           iteration++) {
        z_y = 2 * z_x * z_y + c_y;
        z_x = z_x_squared - z_y_squared + c_x;

        z_x_squared = z_x * z_x;
        z_y_squared = z_y * z_y;
      };

      local_iteration_buffer[i] = iteration;
    }
  }
}

void compute_mandelbrot(int taskid, int numtasks) {
  int chunksize = (i_x_max * i_y_max) / numtasks;
  int i, j, init_k, x, y;
  int *local_iteration_buffer;
  MPI_Status status;

  local_iteration_buffer = (int *)malloc(chunksize * sizeof(int));
  if (taskid == MASTER) {
    init_k = 0;
    compute_mandelbrot_kernel(local_iteration_buffer, init_k, chunksize);

    for (j = 0; j < chunksize; j++) {
      x = (init_k + j) % i_x_max;
      y = (init_k + j) / i_x_max;
      update_rgb_buffer(local_iteration_buffer[j], x, y);
    }

    for (i = 1; i < numtasks; i++) {
      MPI_Recv(local_iteration_buffer, chunksize, MPI_INT, i, 0, MPI_COMM_WORLD,
               &status);
      init_k = i * chunksize;
      for (j = 0; j < chunksize; j++) {
        x = (init_k + j) % i_x_max;
        y = (init_k + j) / i_x_max;
        update_rgb_buffer(local_iteration_buffer[j], x, y);
      }
    }
  } else {
    init_k = taskid * chunksize;
    compute_mandelbrot_kernel(local_iteration_buffer, init_k, chunksize);
    MPI_Send(local_iteration_buffer, chunksize, MPI_INT, MASTER, 0,
             MPI_COMM_WORLD);
  }
  free(local_iteration_buffer);
}

int main(int argc, char *argv[]) {
  int numtasks, taskid;

  MPI_Init(&argc, &argv); /* Obtain number of tasks and task ID */
  MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
  MPI_Comm_rank(MPI_COMM_WORLD, &taskid);

  init(argc, argv);

  if ((i_y_max * i_x_max) % numtasks != 0) {
    printf("Número de tasks tem que ser um divisor de %d\n",
           (i_y_max * i_x_max));
    exit(0);
  }

  if (taskid == MASTER) {
    allocate_image_buffer();
  }

  compute_mandelbrot(taskid, numtasks);

  if (taskid == MASTER) {
    write_to_file();
  }

  MPI_Finalize();
  return 0;
};
