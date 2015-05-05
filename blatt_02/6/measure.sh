#!/bin/sh

#SBATCH -N2
#SBATCH --error measurement-%A.err
#SBATCH --output measurement-%A.out

echo "matrix-scalar multiplication"
for i in 1 2 4 8 16
do
    echo "$i processes"
    echo "mpirun -np $i ./matrix -s matrix-a-bin scalar matrix-s-out-$i-bin"
    mpirun -np $i ./matrix -s matrix-a-bin scalar-bin matrix-out-s-$i-bin
    echo ""
done

echo "matrix-matrix multiplication"
for i in 1 2 4 8 16
do
    echo "$i processes"
    echo "mpirun -np $i ./matrix -m matrix-a-bin matrix-b-bin matrix-m-out-$i-bin"
    mpirun -np $i ./matrix -m matrix-a-bin matrix-b-bin matrix-out-m-$i-bin
    echo ""
done
