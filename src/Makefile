OUTPUT=mandelbrot

IMAGE=.ppm

CC=gcc
CC_OPT=-std=c11

.PHONY: all
all: $(OUTPUT)_seq $(OUTPUT)_mpi

$(OUTPUT)_seq: $(OUTPUT)_seq.c
	$(CC) -o $(OUTPUT)_seq $(CC_OPT) $(OUTPUT)_seq.c

$(OUTPUT)_mpi: $(OUTPUT)_mpi.c
	mpicc $(OUTPUT)_mpi.c -o $(OUTPUT)_mpi

.PHONY: clean
clean:
	rm $(OUTPUT)_seq $(OUTPUT)_mpi *$(IMAGE)
