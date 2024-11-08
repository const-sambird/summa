#!/bin/bash

rm -f summa.mpi
module load OpenMPI
mpicc -DPROC_COLS=2 -DPROC_ROWS=2 -DBCAST=long_bcast test_dispatch.c summa.c matrix.c bcast.c -o summa.mpi

