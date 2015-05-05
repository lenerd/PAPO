#include "helpers.h"

#include <stdio.h>


void create_process_info (process_info_t* pinfo, int size, int rank)
{
    pinfo->size = size;
    pinfo->rank = rank;
}


void create_partition (partition_t* part, process_info_t* pinfo, uint64_t n)
{
    uint64_t r = (uint64_t) pinfo->rank;
    uint64_t s = (uint64_t) pinfo->size;
    uint64_t base_len, rest;
    base_len = n / s;
    rest = n % s;
    part->start = r * base_len;
    part->start += r < rest ? r : rest;
    part->end = (r + 1) * base_len;
    part->end += (r + 1) < rest ? (r + 1) : rest;
    part->len = part->end - part->start;
    part->max_len = base_len + rest;
}


uint64_t skip_lines (FILE* f, uint64_t lines)
{
    int c;
    uint64_t cnt = 0;
    while (cnt < lines && (c = fgetc(f)) != EOF)
    {
        if (c == '\n')
            ++cnt;
    }
    return cnt;
}


double scalar_read (char* path)
{
    FILE* file;
    double s;

    /* open file */
    file = fopen(path, "r");
    if (!file)
    {
        perror("File opening failed\n");
        return 0;
    }

    /* scan matrix dimensions */
    if (fscanf(file, "%lf\n", &s) != 1)
    {
        fprintf(stderr, "invalid file\n");
        fclose(file);
        return 0;
    }

    /* cleanup */
    fclose(file);
    return s;
}


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
    dst->tv_nsec -= src_a->tv_nsec - src_b->tv_nsec;
    if (dst->tv_nsec >= 1000000000)
    {
        ++dst->tv_sec;
        dst->tv_nsec %= 1000000000;
    }
}


double time_to_double (struct timespec* src)
{
    double d;
    d = (double) src->tv_nsec;
    d /= 1000000000;
    d += (double) src->tv_sec;
    return d;
}
