#ifndef MATRIX_MPI_H
#define MATRIX_MPI_H

#include "helpers.h"

#include <stdint.h>


/**
 * Struct representing a matrix.
 */
typedef struct
{
    uint64_t rows;
    partition_t row_part;
    uint64_t cols;
    int64_t* data;
    int64_t** m;
} matrix_t;


/**
 * Calculates matrix vector product A*x.
 */
matrix_t* AB (matrix_t* A, matrix_t* B);


/**
 * Creates empty matrix wit given dimensions.
 */
matrix_t* create_matrix (uint64_t rows, uint64_t cols, process_info_t* pinfo);

/**
 * Reads matrix from file.
 */
matrix_t* read_matrix (char* path, process_info_t* pinfo);

/**
 * Writes matrix to file.
 */
void write_matrix (char* path, matrix_t* A, process_info_t* pinfo);

/**
 * Destroys a matrix.
 */
void destroy_matrix (matrix_t* A);

/*
 * Prints a matrix.
 */
void print_matrix (matrix_t* A, process_info_t* pinfo);

#endif // MATRIX_MPI_H
