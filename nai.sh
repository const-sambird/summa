#!/bin/bash

#SBATCH --partition=normal
#SBATCH --exclusive
#SBATCH --ntasks=1
#SBATCH --mem=16G
#SBATCH --output="nai_stdout.txt"
#SBATCH --error="nai_stderr.txt"
#SBATCH --time=00:30:00
#SBATCH --job-name="matmul_bench"
#SBATCH --mail-user=sam.bird@ou.edu
#SBATCH --mail-type=ALL
#SBATCH --chdir=/home/sbird/tensor

./naive.x

