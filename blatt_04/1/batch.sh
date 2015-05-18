#!/bin/sh
#SBATCH -N1
#SBATCH -n1
#SBATCH --output sum-%A.out

export OMP_NUM_THREADS=12
for i in 1 2 3 4; do
    srun ./1.${i}.out $(seq 1 10000)
done
