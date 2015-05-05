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
