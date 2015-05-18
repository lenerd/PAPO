#define _POSIX_C_SOURCE 199309L
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <omp.h>
#include "time_difference.h"


int main (int argc, char** argv)
{
    int sum = 0;
    int local_sum = 0;
    struct timespec start, end, diff;

    clock_gettime(CLOCK_REALTIME, &start);

    #pragma omp parallel private(local_sum)
    {
        #pragma omp for
        for (int i = 1; i < argc; ++i)
            local_sum += atoi(argv[i]);

        #pragma omp atomic
        sum += local_sum;
    }

    clock_gettime(CLOCK_REALTIME, &end);

    time_difference(&start, &end, &diff);

    printf("sum = %d\n", sum);
    printf("t = %ld,%09ld\n", diff.tv_sec, diff.tv_nsec);

    return EXIT_SUCCESS;
}
