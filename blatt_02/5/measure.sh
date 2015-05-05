#!/bin/sh

#SBATCH -N2
#SBATCH --error measurement-%A.err
#SBATCH --output measurement-%A.out

echo "matrix-scalar multiplacation"
for i in 1 2 4 8 16
do
    echo "$i processes"
    mpirun -np $i ./matrix -s matrix-a scalar matrix-out-$1
    echo ""
done

echo "matrix-matrix multiplacation"
for i in 1 2 4 8 16
do
    echo "$i processes"
    mpirun -np $i ./matrix -m matrix-a matrix-b matrix-out-$1
    echo ""
done
