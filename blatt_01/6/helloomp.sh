#!/bin/sh
#SBATCH -N1
#SBATCH -n1

export OMP_NUM_THREADS=8
srun ./helloomp
