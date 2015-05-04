#include "matrix_mpi.h"
#include "helpers.h"

#include <assert.h>
#include <mpi.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>



// matrix_t* AB (matrix_t* A, matrix_t* B)
// {
//     assert(A->cols == B->rows);
//     matrix_t* C = create_matrix(A->rows, B->cols);
//     uint64_t i, j, k;
//     for (i = 0; i < A->rows; ++i)
//         for (j = 0; j < B->cols; ++j)
//             for (k = 0; k < A->cols; ++k)
//                 C->m[i][j] += A->m[i][k] * B->m[k][j];
//     return C;
// }


matrix_t* create_matrix (uint64_t rows, uint64_t cols, process_info_t* pinfo)
{
    matrix_t* A;

    A = (matrix_t*) malloc(sizeof(matrix_t));
    if (A == NULL)
    {
        fprintf(stderr, "memory allocation failed\n");
        return NULL;
    }

    create_partition(&A->row_part, pinfo, rows);

    A->rows = rows;
    A->cols = cols;
    A->data = calloc(A->row_part.len *  cols, sizeof(int64_t));
    A->m = calloc(rows, sizeof(int64_t*));

    if (A->data == NULL || A->m == NULL)
    {
        fprintf(stderr, "memory allocation failed\n");
        free(A);
        return NULL;
    }
    for (uint64_t i = A->row_part.start; i < A->row_part.end; ++i)
    {
        A->m[i] = A->data + (i - A->row_part.start) * cols;
    }
    return A;
}


matrix_t* read_matrix (char* path, process_info_t* pinfo)
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
    create_partition(&A->row_part, pinfo, rows);
    A->data = calloc(A->row_part.max_len * cols, sizeof(int64_t));
    A->m = calloc(rows, sizeof(int64_t*));
    if (A->data == NULL || A->m == NULL)
    {
        fprintf(stderr, "memory allocation failed\n");
        fclose(file);
        free(A);
        return NULL;
    }
    for (uint64_t i = A->row_part.start; i < A->row_part.end; ++i)
    {
        A->m[i] = A->data + (i - A->row_part.start) * cols;
    }
    skip_lines(file, A->row_part.start);
    for (uint64_t i = A->row_part.start; i < A->row_part.end; ++i)
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


void write_matrix (char* path, matrix_t* A, process_info_t* pinfo)
{
    FILE* file = NULL;
    int state = -1;

    if (pinfo->rank == 0)
    {
        /* creating file */
        file = fopen(path, "w");
        if (file == NULL)
        {
            perror("File opening failed\n");
            state = -1;
            if (pinfo->size > 1)
            {
                MPI_Send(&state, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
            }
            return;
        }
        /* write file header */
        if ((state = fprintf(file, "%lu %lu\n", A->rows, A->cols)) < 0)
        {
            fprintf(stderr, "error writing to file\n");
            fclose(file);
            if (pinfo->size > 1)
            {
                MPI_Send(&state, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
            }
            return;
        }
    }
    else // pinfo->rank != 0
    {
        /* wait for last process */
        MPI_Recv(&state, 1, MPI_INT, pinfo->rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        if (state < 0)
        {
            fprintf(stderr, "recv: error writing to file\n");
            if (pinfo->rank < pinfo->size - 1)
            {
                MPI_Send(&state, 1, MPI_INT, pinfo->rank + 1, 0, MPI_COMM_WORLD);
            }
            return;
        }
        /* appending to file file */
        file = fopen(path, "a");
        if (file == NULL)
        {
            perror("File opening failed\n");
            state = -1;
            if (pinfo->size > 1)
            {
                MPI_Send(&state, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
            }
            return;
        }
    }
    /* writing local part */
    for (uint64_t i = A->row_part.start; i < A->row_part.end; ++i)
    {
        for (uint64_t j = 0; j < A->cols; ++j)
        {
            if ((state = fprintf(file, "%ld ", A->m[i][j])) < 0)
            {
                fprintf(stderr, "error writing to file\n");
                fclose(file);
                if (pinfo->rank < pinfo->size - 1)
                {
                    MPI_Send(&state, 1, MPI_INT, pinfo->rank + 1, 0, MPI_COMM_WORLD);
                }
                return;
            }
        }
        if ((state = fprintf(file, "\n")) < 0)
        {
            fprintf(stderr, "error writing to file\n");
            fclose(file);
            if (pinfo->rank < pinfo->size - 1)
            {
                MPI_Send(&state, 1, MPI_INT, pinfo->rank + 1, 0, MPI_COMM_WORLD);
            }
            return;
        }
    }
    fclose(file);
    /* tell next process to go on */
    if (pinfo->rank != pinfo->size - 1)
    {
        MPI_Send(&state, 1, MPI_INT, pinfo->rank + 1, 0, MPI_COMM_WORLD);
    }
}


void destroy_matrix (matrix_t* A)
{
    free(A->m);
    free(A->data);
    free(A);
}


void print_matrix (matrix_t* A, process_info_t* pinfo)
{
    if (pinfo->rank == 0)
    {
        printf("[Matrix: %lu x %lu]\n", A->rows, A->cols);
    }
    if (pinfo->rank > 0)
    {
        MPI_Recv(NULL, 0, MPI_C_BOOL, pinfo->rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
    for (uint64_t i = A->row_part.start; i < A->row_part.end; ++i)
    {
        for (uint64_t j = 0; j < A->cols; ++j)
            printf("%ld ", A->m[i][j]);
        printf("\n");
    }
    if (pinfo->rank < pinfo->size - 1)
    {
        MPI_Send(NULL, 0, MPI_C_BOOL, pinfo->rank + 1, 0, MPI_COMM_WORLD);
    }
}
