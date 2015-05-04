#include <stdint.h>

/**
 * Struct representing a matrix.
 */
typedef struct
{
    uint64_t rows;
    uint64_t cols;
    int64_t* data;
    int64_t** m;
} matrix_t;

/**
 * Struct representing a vector.
 */
typedef struct
{
    uint64_t len;
    int64_t* data;
} vector_t;

/**
 * Calculates matrix vector product A*x.
 */
vector_t* Ax (matrix_t* A, vector_t* x);

/**
 * Calculates matrix vector product A*x.
 */
matrix_t* AB (matrix_t* A, matrix_t* B);

/** 
 * Creates an empty vector with length len.
 */
vector_t* create_vector (uint64_t len);

/**
 * Reads a vector from file.
 */
vector_t* read_vector (char* path);

/**
 * Writes a vector to file.
 */
void write_vector (char* path, vector_t* x);

/**
 * Destroys a vector.
 */
void destroy_vector (vector_t* x);

/**
 * Prints a vector.
 */
void print_vector (vector_t* x);


/**
 * Creates empty matrix wit given dimensions.
 */
matrix_t* create_matrix (uint64_t rows, uint64_t cols);

/**
 * Reads matrix from file.
 */
matrix_t* read_matrix (char* path);

/**
 * Writes matrix to file.
 */
void write_matrix (char* path, matrix_t* A);

/**
 * Destroys a matrix.
 */
void destroy_matrix (matrix_t* A);

/*
 * Prints a matrix.
 */
void print_matrix (matrix_t* A);
