#ifndef HELPERS_H
#define HELPERS_H

#define _POSIX_C_SOURCE 199309L

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>


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
 * Count rows in a file.
 */
uint64_t count_rows (FILE* file);


/**
 * Count rows in a file.
 */
uint64_t count_cols (FILE* file);


/**
 * Skip n lines in file.
 */
uint64_t skip_lines (FILE* f, uint64_t lines);


/**
 * Reads a scalar value from a file.
 */
double scalar_read (char* path);
double scalar_read_mpiio (char* path);


/**
 * Calculates difference between two measurements.
 */
void time_difference (struct timespec* src_a, struct timespec* src_b, struct timespec* dst);


/**
 * Converts time value to double.
 */
double time_to_double (struct timespec* src);

#endif // HELPERS_H
