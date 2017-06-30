OUTPUT=mandelbrot

IMAGE=.ppm

CC=gcc
CC_OPT=-std=c11

.PHONY: all
all: $(OUTPUT)_seq $(OUTPUT)_seq_io $(OUTPUT)_mpi $(OUTPUT)_mpi_io

$(OUTPUT)_seq: $(OUTPUT)_seq.c
	$(CC) -o $(OUTPUT)_seq $(CC_OPT) $(OUTPUT)_seq.c

$(OUTPUT)_seq_io: $(OUTPUT)_seq_io.c
	$(CC) -o $(OUTPUT)_seq_io $(CC_OPT) $(OUTPUT)_seq_io.c

$(OUTPUT)_mpi: $(OUTPUT)_mpi.c
	mpicc $(OUTPUT)_mpi.c -o $(OUTPUT)_mpi $(CC_OPT)

$(OUTPUT)_mpi_io: $(OUTPUT)_mpi_io.c
	mpicc $(OUTPUT)_mpi_io.c -o $(OUTPUT)_mpi_io $(CC_OPT)

.PHONY: clean
clean:
	rm $(OUTPUT)_seq $(OUTPUT)_seq_io $(OUTPUT)_mpi $(OUTPUT)_mpi_io *$(IMAGE)
