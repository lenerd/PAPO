#include "time_difference.h"
#include <time.h>


void time_difference (struct timespec* src_a, struct timespec* src_b, struct timespec* dst)
{
    if (src_a->tv_sec < src_b->tv_sec
            || ((src_a->tv_sec == src_b->tv_sec) && (src_a->tv_nsec < src_b->tv_nsec)))
    {
        struct timespec* tmp = src_a;
        src_a = src_b;
        src_b = tmp;
    }
    dst->tv_sec = src_a->tv_sec - src_b->tv_sec;
    if (src_a->tv_nsec < src_b->tv_nsec)
    {
        --dst->tv_sec;
        dst->tv_nsec = 1000000000;
    }
    dst->tv_nsec += src_a->tv_nsec;
    dst->tv_nsec -= src_b->tv_nsec;
    if (dst->tv_nsec >= 1000000000)
    {
        ++dst->tv_sec;
        dst->tv_nsec %= 1000000000;
    }
}
