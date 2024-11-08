#!/bin/bash

#SBATCH --partition=debug_5min
#SBATCH --exclusive
#SBATCH --ntasks=4
#SBATCH --mem=512M
#SBATCH --output="test_stdout.txt"
#SBATCH --error="test_stderr.txt"
#SBATCH --time=00:05:00
#SBATCH --job-name="summa_test"
#SBATCH --mail-user=sam.bird@ou.edu
#SBATCH --mail-type=ALL
#SBATCH --chdir=/home/sbird/tensor

module load OpenMPI
mpirun summa.mpi 4 4 4

