#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>


int main (int argc, char** argv)
{
    int rank;   /* process id */
    int P;      /* number of processes */
    int ret;    /* return values */

    if ((ret = MPI_Init(&argc, &argv)) != MPI_SUCCESS)
    {
        printf("Error initializing MPI\n");
        MPI_Abort(MPI_COMM_WORLD, ret);
    }

    MPI_Comm_size(MPI_COMM_WORLD, &P);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    printf("Hello World %d von %d\n", rank, P);

    MPI_Finalize();

    return EXIT_SUCCESS;
}
