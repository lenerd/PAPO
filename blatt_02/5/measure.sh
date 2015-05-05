#!/bin/sh

#SBATCH -N2
#SBATCH --error measurement-%A.err
#SBATCH --output measurement-%A.out

echo "matrix-scalar multiplication"
for i in 1 2 4 8 16
do
    echo "$i processes"
    echo "mpirun -np $i ./matrix -s matrix-a scalar matrix-s-out-$i"
    mpirun -np $i ./matrix -s matrix-a scalar matrix-out-s-$i
    echo ""
done

echo "matrix-matrix multiplication"
for i in 1 2 4 8 16
do
    echo "$i processes"
    echo "mpirun -np $i ./matrix -m matrix-a matrix-b matrix-m-out-$i"
    mpirun -np $i ./matrix -m matrix-a matrix-b matrix-out-m-$i
    echo ""
done
