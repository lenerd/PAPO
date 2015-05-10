#!/bin/sh
#SBATCH -N1
#SBATCH --output speedup-%A.out

echo "# processes, seconds"
for i in $(seq 1 12)
do
    echo "$i, $(/usr/bin/time -p mpirun -np $i ./mpi-speedup 2>&1 > /dev/null \
        | grep real | cut -d ' ' -f 2)"
done
