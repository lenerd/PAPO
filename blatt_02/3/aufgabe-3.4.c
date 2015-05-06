#include <mpi.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>


int main (int argc, char** argv)
{
    int rank;   /* process id */
    int P;      /* number of processes */
    int ret;    /* return values */
    int64_t* buffer0;
    int64_t* buffer1;
    uint8_t order = 0;

    if ((ret = MPI_Init(&argc, &argv)) != MPI_SUCCESS)
    {
        printf("Error initializing MPI\n");
        MPI_Abort(MPI_COMM_WORLD, ret);
    }

    MPI_Comm_size(MPI_COMM_WORLD, &P);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (P == 1)  /* nothing to do alone */
        return EXIT_FAILURE;
    if (argc > 1)
        order = (uint8_t) atoi(argv[1]) % 4;

    if (rank < 2)
    {
        buffer0 = malloc(1000000 * sizeof(int64_t));
        buffer1 = malloc(1000000 * sizeof(int64_t));
        if (buffer0 == NULL || buffer1 == NULL)
        {
            printf("memory allocation failed\n");
            MPI_Abort(MPI_COMM_WORLD, -1);
        }
    }

    /* Bei order == 0x1 und order == 0x2 kann es zu Deadlocks kommen. */
    if (rank == 0)
    {
        if (!(0x1 & order))
        {
            MPI_Send(buffer0, 1000000, MPI_INT64_T, 1, 0, MPI_COMM_WORLD);
            MPI_Recv(buffer1, 1000000, MPI_INT64_T, 1, 1, MPI_COMM_WORLD, NULL);
        }
        else
        {
            MPI_Recv(buffer1, 1000000, MPI_INT64_T, 1, 1, MPI_COMM_WORLD, NULL);
            MPI_Send(buffer0, 1000000, MPI_INT64_T, 1, 0, MPI_COMM_WORLD);
        }
    }
    else if (rank == 1)
    {
        if (!(0x2 & order))
        {
            MPI_Recv(buffer0, 1000000, MPI_INT64_T, 0, 0, MPI_COMM_WORLD, NULL);
            MPI_Send(buffer1, 1000000, MPI_INT64_T, 0, 1, MPI_COMM_WORLD);
        }
        else
        {
            MPI_Send(buffer1, 1000000, MPI_INT64_T, 0, 1, MPI_COMM_WORLD);
            MPI_Recv(buffer0, 1000000, MPI_INT64_T, 0, 0, MPI_COMM_WORLD, NULL);
        }
    }

    if (rank < 2)
    {
        free(buffer0);
        free(buffer1);
    }

    MPI_Finalize();

    printf("Bye from %d of %d\n", rank, P);

    return EXIT_SUCCESS;
}
