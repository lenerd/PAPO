#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "matrix_serial.h"


vector_t* Ax (matrix_t* A, vector_t* x)
{
    assert(A->cols == x->len);
    vector_t* z;
    z = create_vector(A->rows);
    for (uint64_t i = 0; i < A->rows; ++i)
    {
        z->data[i] = 0;
        for (uint64_t j = 0; j < A->cols; ++j)
        {
            z->data[i] += A->m[i][j] * x->data[j];
        }
    }
    return z;
}


matrix_t* AB (matrix_t* A, matrix_t* B)
{
    assert(A->cols == B->rows);
    matrix_t* C = create_matrix(A->rows, B->cols);
    uint64_t i, j, k;
    for (i = 0; i < A->rows; ++i)
        for (j = 0; j < B->cols; ++j)
            for (k = 0; k < A->cols; ++k)
                C->m[i][j] += A->m[i][k] * B->m[k][j];
    return C;
}


vector_t* create_vector (uint64_t len)
{
    vector_t* x;
    x = (vector_t*) malloc(sizeof(vector_t));
    if (x == NULL)
    {
        fprintf(stderr, "memory allocation failed\n");
        return NULL;
    }
    x->len = len;
    x->data = calloc(len, sizeof(int64_t));
    if (x->data == NULL)
    {
        fprintf(stderr, "memory allocation failed\n");
        free(x);
        return NULL;
    }
    return x;
}


vector_t* read_vector (char* path)
{
    vector_t* x;
    uint64_t len;
    FILE* file;
    x = (vector_t*) malloc(sizeof(vector_t));
    if (x == NULL)
    {
        fprintf(stderr, "memory allocation failed\n");
        return NULL;
    }
    file = fopen(path, "r");
    if (!file)
    {
        perror("File opening failed\n");
        free(x);
        return NULL;
    }
    if (fscanf(file, "%lu\n", &len) != 1 || len == 0)
    {
        fprintf(stderr, "invalid file\n");
        fclose(file);
        free(x);
        return NULL;
    }
    x->len = len;
    x->data = calloc(len, sizeof(int64_t));
    if (x->data == NULL)
    {
        fprintf(stderr, "memory allocation failed\n");
        fclose(file);
        free(x);
        return NULL;
    }
    for (uint64_t i = 0; i < len; ++i)
    {
        if (fscanf(file, "%ld\n", x->data + i) != 1)
        {
            fprintf(stderr, "invalid file\n");
            fclose(file);
            free(x->data);
            free(x);
            return NULL;
        }
    }
        
    fclose(file);
    return x;
}


void write_vector (char* path, vector_t* x)
{
    FILE* file;
    file = fopen(path, "w");
    if (!file)
    {
        perror("File opening failed\n");
    }
    if (!fprintf(file, "%lu\n", x->len))
    {
        fprintf(stderr, "error writing to file\n");
        fclose(file);
    }
    for (uint64_t i = 0; i < x->len; ++i)
    {
        if (!fprintf(file, "%ld\n", x->data[i]))
        {
            fprintf(stderr, "error writing to file\n");
            fclose(file);
        }
    }
        
    fclose(file);
}


void destroy_vector (vector_t* x)
{
    free(x->data);
    free(x);
}


void print_vector (vector_t* x)
{
    printf("[Vector: %lu]\n", x->len);
    for (uint64_t i = 0; i < x->len; ++i)
        printf("%ld\n", x->data[i]);
}


matrix_t* create_matrix (uint64_t rows, uint64_t cols)
{
    matrix_t* A;
    A = (matrix_t*) malloc(sizeof(matrix_t));
    if (A == NULL)
    {
        fprintf(stderr, "memory allocation failed\n");
        return NULL;
    }
    A->rows = rows;
    A->cols = cols;
    A->data = calloc(rows *  cols, sizeof(int64_t));
    A->m = malloc(rows * sizeof(int64_t*));
    if (A->data == NULL || A->m == NULL)
    {
        fprintf(stderr, "memory allocation failed\n");
        free(A);
        return NULL;
    }
    for (uint64_t i = 0; i < rows; ++i)
    {
        A->m[i] = A->data + i * cols;
    }
    return A;
}


matrix_t* read_matrix (char* path)
{
    matrix_t* A;
    uint64_t rows, cols;
    FILE* file;
    A = (matrix_t*) malloc(sizeof(matrix_t));
    if (A == NULL)
    {
        fprintf(stderr, "memory allocation failed\n");
        return NULL;
    }
    file = fopen(path, "r");
    if (!file)
    {
        perror("File opening failed\n");
        free(A);
        return NULL;
    }
    if (fscanf(file, "%lu %lu\n", &rows, &cols) != 2 || rows == 0 || cols == 0)
    {
        fprintf(stderr, "invalid file\n");
        fclose(file);
        free(A);
        return NULL;
    }
    A->rows = rows;
    A->cols = cols;
    A->data = calloc(rows * cols, sizeof(int64_t));
    A->m = malloc(rows * sizeof(int64_t*));
    if (A->data == NULL || A->m == NULL)
    {
        fprintf(stderr, "memory allocation failed\n");
        fclose(file);
        free(A);
        return NULL;
    }
    for (uint64_t i = 0; i < rows; ++i)
    {
        A->m[i] = A->data + i * cols;
    }
    for (uint64_t i = 0; i < rows; ++i)
    {
        for (uint64_t j = 0; j < cols; ++j)
        {
            if (fscanf(file, "%ld", &A->m[i][j]) != 1)
            {
                fprintf(stderr, "invalid file\n");
                fclose(file);
                free(A->m);
                free(A->data);
                free(A);
                return NULL;
            }
        }
    }
        
    fclose(file);
    return A;
}


void write_matrix (char* path, matrix_t* A)
{
    FILE* file;
    file = fopen(path, "w");
    if (!file)
    {
        perror("File opening failed\n");
    }
    if (!fprintf(file, "%lu %lu\n", A->rows, A->cols))
    {
        fprintf(stderr, "error writing to file\n");
        fclose(file);
    }
    for (uint64_t i = 0; i < A->rows; ++i)
    {
        for (uint64_t j = 0; j < A->cols; ++j)
        {
            if (!fprintf(file, "%ld ", A->m[i][j]))
            {
                fprintf(stderr, "error writing to file\n");
                fclose(file);
            }
        }
        if (!fprintf(file, "\n"))
        {
            fprintf(stderr, "error writing to file\n");
            fclose(file);
        }
    }
        
    fclose(file);
}


void destroy_matrix (matrix_t* A)
{
    free(A->m);
    free(A->data);
    free(A);
}


void print_matrix (matrix_t* A)
{
    printf("[Matrix: %lu x %lu]\n", A->rows, A->cols);
    for (uint64_t i = 0; i < A->rows; ++i)
    {
        for (uint64_t j = 0; j < A->cols; ++j)
            printf("%ld ", A->m[i][j]);
        printf("\n");
    }
}
