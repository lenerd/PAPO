#define _POSIX_C_SOURCE 199309L
#include "matrix_mpi.h"
#include "helpers.h"
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


void usage (void);


int main (int argc, char** argv)
{
    int ret, rank, size;
    process_info_t pinfo;
    struct timespec start_time, end_time;
    long seconds, min_seconds, max_seconds;

    if ((ret = MPI_Init(&argc, &argv)) != MPI_SUCCESS)
    {
        fprintf(stderr, "Error initializing MPI\n");
        MPI_Abort(MPI_COMM_WORLD, ret);

    }

    clock_gettime(CLOCK_REALTIME, &start_time);

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    create_process_info(&pinfo, size, rank);

    if (argc < 5)
    {
        if (rank == 0)
            usage();
        MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
    }

    /* matrix x matrix */
    if (strcmp(argv[1], "-m") == 0)
    {
        matrix_t* A, * B, * C;
        A = matrix_read(argv[2], &pinfo);
        B = matrix_read(argv[3], &pinfo);
        C = matrix_dot(A, B, &pinfo);
        matrix_write(argv[4], C, &pinfo);
        matrix_destroy(A);
        matrix_destroy(B);
        matrix_destroy(C);
    }
    /* matrix x scalar */
    else if (strcmp(argv[1], "-s") == 0)
    {
        matrix_t* A, * C;
        int64_t s;
        A = matrix_read(argv[2], &pinfo);
        s = scalar_read(argv[3]);
        C = matrix_scalar(A, s);
        matrix_write(argv[4], C, &pinfo);
        matrix_destroy(A);
        matrix_destroy(C);
    }
    else
    {
        if (rank == 0)
            usage();
        MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
    }

    clock_gettime(CLOCK_REALTIME, &end_time);

    seconds = end_time.tv_sec - start_time.tv_sec;
    MPI_Reduce(&seconds, &min_seconds, 1, MPI_LONG, MPI_MIN, 0, MPI_COMM_WORLD);
    MPI_Reduce(&seconds, &max_seconds, 1, MPI_LONG, MPI_MAX, 0, MPI_COMM_WORLD);

    if (rank == 0)
    {
        printf("t_min = %ld s\n", min_seconds);
        printf("t_max = %ld s\n", max_seconds);
    }

    MPI_Finalize();

    return EXIT_SUCCESS;
}


void usage (void)
{
    printf("Usage:\n");
    printf("Matrix matrix multiplication\n");
    printf("    matrix -m file_A file_B file_AxB\n");
    printf("Matrix scalar multiplication\n");
    printf("    matrix -s file_A file_s file_Axs\n");
}
