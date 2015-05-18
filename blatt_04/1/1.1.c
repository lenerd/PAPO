#define _POSIX_C_SOURCE 199309L
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <omp.h>
#include "time_difference.h"


int main (int argc, char** argv)
{
    int64_t sum = 0;
    struct timespec start, end, diff;
    int t;
    int threads = omp_get_max_threads();
    int64_t* local_sums = calloc((size_t)threads, sizeof(int));

    clock_gettime(CLOCK_REALTIME, &start);

    #pragma omp parallel private(t)
    {
        t = omp_get_thread_num();

        #pragma omp for
        for (int i = 1; i < argc; ++i)
            local_sums[t] += atoi(argv[i]);
    }

    for (int t = 0; t < threads; ++t)
        sum += local_sums[t];

    clock_gettime(CLOCK_REALTIME, &end);

    time_difference(&start, &end, &diff);

    printf("sum = %ld\n", sum);
    printf("t = %ld,%09ld\n", diff.tv_sec, diff.tv_nsec);

    free(local_sums);

    return EXIT_SUCCESS;
}
