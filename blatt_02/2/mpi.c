#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>


int main (int argc, char** argv)
{
    int rank;   /* process id */
    int P;      /* number of processes */
    int ret;    /* return values */
    int array[5];
    int sum = 0;

    if ((ret = MPI_Init(&argc, &argv)) != MPI_SUCCESS)
    {
        printf("Error initializing MPI\n");
        MPI_Abort(MPI_COMM_WORLD, ret);
    }

    MPI_Comm_size(MPI_COMM_WORLD, &P);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    for (int i = 0; i < 5; ++i)
        array[i] = rank + i;

    for (int i = 0; i < 5; ++i)
        sum += array[i];
    printf("Hello World %d von %d: %d\n", rank, P, sum);

    MPI_Finalize();

    return EXIT_SUCCESS;
}
