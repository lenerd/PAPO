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
 * Calculates scalar product A*s.
 */
matrix_t* matrix_scalar (matrix_t* A, int64_t s);

/**
 * Calculates matrix product A*B.
 */
matrix_t* matrix_dot (matrix_t* A, matrix_t* B, process_info_t* pinfo);

/**
 * Copys a matrix.
 */
matrix_t* matrix_copy (matrix_t* A);


/**
 * Creates empty matrix wit given dimensions.
 */
matrix_t* matrix_create (uint64_t rows, uint64_t cols, process_info_t* pinfo);

/**
 * Reads matrix from file.
 */
matrix_t* matrix_read (char* path, process_info_t* pinfo);

/**
 * Writes matrix to file.
 */
void matrix_write (char* path, matrix_t* A, process_info_t* pinfo);

/**
 * Destroys a matrix.
 */
void matrix_destroy (matrix_t* A);

/*
 * Prints a matrix.
 */
void matrix_print (matrix_t* A, process_info_t* pinfo);

#endif // MATRIX_MPI_H
