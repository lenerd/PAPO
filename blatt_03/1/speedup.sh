#!/bin/sh
#SBATCH -N1
#SBATCH --output speedup-%A.out

echo "# processes, measurements 1 to 3 in seconds"
for i in $(seq 1 12)
do
    m1=$(/usr/bin/time -p mpirun -np $i ./mpi-speedup 2>&1 > /dev/null \
        | grep real | cut -d ' ' -f 2)
    m2=$(/usr/bin/time -p mpirun -np $i ./mpi-speedup 2>&1 > /dev/null \
        | grep real | cut -d ' ' -f 2)
    m3=$(/usr/bin/time -p mpirun -np $i ./mpi-speedup 2>&1 > /dev/null \
        | grep real | cut -d ' ' -f 2)
    echo "$i $m1 $m2 $m2"
done
