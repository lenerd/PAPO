#ifndef HELPERS_H
#define HELPERS_H

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>


/**
 * Struct representing part of a range.
 */
typedef struct 
{
    uint64_t start; /* inclusive */
    uint64_t end;   /* exclusive */
    uint64_t len;
    uint64_t max_len;
} partition_t;


/**
 * Struct with info about existing processes.
 */
typedef struct
{
    int size;
    int rank;
} process_info_t;


/**
 * Creates process info.
 */
void create_process_info (process_info_t* pinfo, int size, int rank);


/**
 * Creates partition.
 */
void create_partition (partition_t* part, process_info_t* pinfo, uint64_t n);


/**
 * Skip n lines in file.
 */
uint64_t skip_lines (FILE* f, uint64_t lines);


/**
 * Reads a scalar value from a file.
 */
int64_t scalar_read (char* path);

#endif // HELPERS_H
