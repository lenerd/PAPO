#ifndef TIME_DIFFERENCE_H
#define TIME_DIFFERENCE_H

#define _POSIX_C_SOURCE 199309L

#include <time.h>

void time_difference (struct timespec* src_a, struct timespec* src_b, struct timespec* dst);

#endif /* TIME_DIFFERENCE_H */
