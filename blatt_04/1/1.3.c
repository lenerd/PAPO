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

    clock_gettime(CLOCK_REALTIME, &start);

    #pragma omp parallel for reduction(+:sum)
    for (int i = 1; i < argc; ++i)
        sum += atoi(argv[i]);

    clock_gettime(CLOCK_REALTIME, &end);

    time_difference(&start, &end, &diff);

    printf("sum = %ld\n", sum);
    printf("t = %ld,%09ld\n", diff.tv_sec, diff.tv_nsec);

    return EXIT_SUCCESS;
}
