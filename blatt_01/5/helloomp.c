#include <omp.h>
#include <stdio.h>
#include <stdlib.h>


/*
 * run as OMP_NUM_THREADS=42 ./helloomp
 */
int main (void)
{
    int rank;   /* process id */
    int P;      /* number of processes */


    #pragma omp parallel private(rank, P)
    {
        rank = omp_get_thread_num();
        P = omp_get_num_threads();
        printf("Hello World %d von %d\n", rank, P);
    }


    return EXIT_SUCCESS;
}
