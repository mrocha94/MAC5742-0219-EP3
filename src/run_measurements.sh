#! /bin/bash

set -o xtrace

MEASUREMENTS=10
SIZE=8192

NAMES=('mandelbrot_mpi')

make
# mkdir results

# mkdir results/mandelbrot_seq
# perf stat -r $MEASUREMENTS ./mandelbrot_seq -2.5 1.5 -2.0 2.0 $SIZE >> full.log 2>&1
# perf stat -r $MEASUREMENTS ./mandelbrot_seq -0.8 -0.7 0.05 0.15 $SIZE >> seahorse.log 2>&1
# perf stat -r $MEASUREMENTS ./mandelbrot_seq 0.175 0.375 -0.1 0.1 $SIZE >> elephant.log 2>&1
# perf stat -r $MEASUREMENTS ./mandelbrot_seq -0.188 -0.012 0.554 0.754 $SIZE >> triple_spiral.log 2>&1
# mv *.log results/mandelbrot_seq

INSTANCES=1
mkdir results/mandelbrot_mpi_$INSTANCES
perf stat -r $MEASUREMENTS mpirun -np 8 mandelbrot_mpi -2.5 1.5 -2.0 2.0 $SIZE >> full.log 2>&1
perf stat -r $MEASUREMENTS mpirun -np 8 mandelbrot_mpi -0.8 -0.7 0.05 0.15 $SIZE >> seahorse.log 2>&1
perf stat -r $MEASUREMENTS mpirun -np 8 mandelbrot_mpi 0.175 0.375 -0.1 0.1 $SIZE >> elephant.log 2>&1
perf stat -r $MEASUREMENTS mpirun -np 8 mandelbrot_mpi -0.188 -0.012 0.554 0.754 $SIZE >> triple_spiral.log 2>&1
mv *.log results/mandelbrot_mpi_$INSTANCES

rm *.ppm
done
