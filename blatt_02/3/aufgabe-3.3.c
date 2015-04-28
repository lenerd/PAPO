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
    int64_t* numbers;    /* given numbers */
    int64_t* num_local;  /* numbers to handle */
    int* sendcnts;
    int* displs;
    int64_t sum;
    int64_t local_sum;
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

    sendcnts = malloc((uint64_t)P * sizeof(int));
    displs = malloc((uint64_t)P * sizeof(int));
    num_local = malloc(part.len * sizeof(int64_t));
    if (num_local == NULL || sendcnts == NULL || displs == NULL)
    {
        printf("memory allocation failed\n");
        MPI_Abort(MPI_COMM_WORLD, -1);
    }
    for (int r = 0; r < P; ++r)
    {
        create_partition(&part_r, n, (uint64_t)P, (uint64_t)r);
        sendcnts[r] = (int)part_r.len;
        displs[r] = (int)part_r.start;
    }
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
    }
    /* distribute numbers */
    MPI_Scatterv(numbers, sendcnts, displs, MPI_INT64_T, num_local, (int)part.len, MPI_INT64_T, 0, MPI_COMM_WORLD);
    free(sendcnts);
    free(displs);
    /* add numbers */
    local_sum = 0;
    for (uint64_t i = 0; i < part.len; ++i)
        local_sum += num_local[i];
    free(num_local);
    MPI_Reduce(&local_sum, &sum, 1, MPI_INT64_T, MPI_SUM, 0, MPI_COMM_WORLD);
    /* Solange jeder Prozess gleich viele Zahlen zu addieren hat, könnte man den
     * dritten Parameter auf die Anzahl der Zahlen pro Prozess setzen.
     * Das vereinfacht das lokale Aufsummieren. */
    if (rank == 0)
    {
        printf("Summe: %" PRIu64 "\n", sum);
        free(numbers);
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
