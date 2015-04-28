#include <inttypes.h>
#include <mpi.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>


typedef struct 
{
    uint64_t start; /* inclusive */
    uint64_t end;   /* exclusive */
    uint64_t len;
} partition;


void create_partition(partition* part, uint64_t n, uint64_t p, uint64_t r);


int main (int argc, char** argv)
{
    int rank;   /* process id */
    int P;      /* number of processes */
    int ret;    /* return values */

    uint64_t n;          /* number of parameter */
    int64_t* numbers;   /* given numbers */
    int64_t tmp;
    int64_t sum;
    partition part;     /* local partition */
    partition part_r;   /* dst partition */

    if (argc == 1) /* keine Parameter */
        return EXIT_SUCCESS;

    if ((ret = MPI_Init(&argc, &argv)) != MPI_SUCCESS)
    {
        printf("Error initializing MPI\n");
        MPI_Abort(MPI_COMM_WORLD, ret);
    }

    MPI_Comm_size(MPI_COMM_WORLD, &P);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    n = (uint64_t)argc - 1;
    create_partition(&part, n, (uint64_t)P, (uint64_t)rank);

    if (rank == 0)
    {
        numbers = malloc(n * sizeof(int64_t));
        if (numbers == NULL)
        {
            printf("memory allocation failed\n");
            MPI_Abort(MPI_COMM_WORLD, -1);
        }
        for (uint64_t i = 0; i < n; ++i)
            numbers[i] = atoi(argv[i + 1]);
        for (int r = 1; r < P; ++r)
        {
            create_partition(&part_r, n, (uint64_t)P, (uint64_t)r);
            /* distribute numbers */
            if (part_r.len > 0)
                MPI_Send(numbers + part_r.start, (int)part_r.len, MPI_INT64_T, r, r, MPI_COMM_WORLD);
        }
        sum = 0;
        for (uint64_t i = 0; i < part.len; ++i)
            sum += numbers[i];
        
        for (int r = 1; r < P; ++r)
        {
            /* recv and add sums */
            MPI_Recv(&tmp, 1, MPI_INT64_T, r, r, MPI_COMM_WORLD, NULL);
            sum += tmp;
        }
        printf("Summe: %" PRIu64 "\n", sum);
        free(numbers);
    }
    else
    {
        tmp = 0;
        if (part.len > 0)
        {
            numbers = malloc(part.len * sizeof(int64_t));
            if (numbers == NULL)
            {
                printf("memory allocation failed\n");
                MPI_Abort(MPI_COMM_WORLD, -1);
            }
            /* recv numbers */
            MPI_Recv(numbers, (int)part.len, MPI_INT64_T, 0, rank, MPI_COMM_WORLD, NULL);
            for (uint64_t i = 0; i < part.len; ++i)
                tmp += numbers[i];
            free(numbers);
        }
        /* send sum */
        MPI_Send(&tmp, 1, MPI_INT64_T, 0, rank, MPI_COMM_WORLD);
    }

    MPI_Finalize();

    return EXIT_SUCCESS;
}


void create_partition(partition* part, uint64_t n, uint64_t p, uint64_t r)
{
    uint64_t base_len, rest;
    base_len = n / p;
    rest = n % p;
    part->start = r * base_len;
    part->start += r < rest ? r : rest;
    part->end = (r + 1) * base_len;
    part->end += (r + 1) < rest ? (r + 1) : rest;
    part->len = part->end - part->start;
}
