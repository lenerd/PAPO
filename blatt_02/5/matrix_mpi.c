#include "matrix_mpi.h"
#include "helpers.h"

#include <assert.h>
#include <mpi.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


matrix_t* matrix_scalar (matrix_t* A, int64_t s)
{
    matrix_t* C;
    C = matrix_copy(A);
    for (uint64_t i = C->row_part.start; i < C->row_part.end; ++i)
    {
        for (uint64_t j = 0; j < C->cols; ++j)
        {
            C->m[i][j] *= s;
        }
    }
    return C;
}


matrix_t* matrix_dot (matrix_t* A, matrix_t* B, process_info_t* pinfo)
{
    /* requirement for matrix multiplication */
    assert(A->cols == B->rows);

    matrix_t* C, * T;
    int rank_from, rank_to;

    /* result matrix */
    C = matrix_create(A->rows, B->cols, pinfo);

    /* calc with local part of B */
    for (uint64_t i = A->row_part.start; i < A->row_part.end; ++i)
        for (uint64_t k = B->row_part.start; k < B->row_part.end; ++k)
            for (uint64_t j = 0; j < B->cols; ++j)
                C->m[i][j] += A->m[i][k] * B->m[k][j];

    /* temp matrix */
    T = matrix_copy(B);
    for (int i = 0; i < pinfo->size - 1; ++i)
    {
        rank_from = (pinfo->rank - 1 + pinfo->size) % pinfo->size;
        rank_to = (pinfo->rank + 1) % pinfo->size;
        /* shift matrix part */
        MPI_Sendrecv_replace(T->data, (int) (T->row_part.max_len * T->cols), MPI_INT64_T, rank_from, 0, rank_to, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        /* shift meta data */
        MPI_Sendrecv_replace(&T->row_part, 4, MPI_UINT64_T, rank_from, 0, rank_to, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        /* reset row pointers */
        memset(T->m, 0, T->rows * sizeof(int64_t*));
        /* calculate new row pointers */
        for (uint64_t i = T->row_part.start; i < T->row_part.end; ++i)
        {
            T->m[i] = T->data + (i - T->row_part.start) * T->cols;
        }

        /* calculate part with next sub matrix */
        for (uint64_t i = A->row_part.start; i < A->row_part.end; ++i)
        {
            for (uint64_t k = T->row_part.start; k < T->row_part.end; ++k)
            {
                for (uint64_t j = 0; j < B->cols; ++j)
                {
                    C->m[i][j] += A->m[i][k] * T->m[k][j];
                }
            }
        }
    }
    matrix_destroy(T);
    return C;
}


matrix_t* matrix_copy (matrix_t* A)
{
    matrix_t* B;

    /* allocate memory for matrix_t struct */
    B = (matrix_t*) malloc(sizeof(matrix_t));
    if (B == NULL)
    {
        fprintf(stderr, "memory allocation failed\n");
        return NULL;
    }
    memcpy((void*) B, (void*) A, sizeof(matrix_t));

    /* initialize matrix struct */
    memcpy((void*) &B->row_part, (void*) &A->row_part, sizeof(partition_t));
    B->rows = A->rows;
    B->cols = A->cols;

    /* allocate memory */
    B->data = calloc(B->row_part.max_len *  B->cols, sizeof(int64_t));
    B->m = calloc(B->rows, sizeof(int64_t*));
    if (B->data == NULL || B->m == NULL)
    {
        fprintf(stderr, "memory allocation failed\n");
        free(B);
        return NULL;
    }
    memcpy((void*) B->data, (void*) A->data, B->row_part.max_len * B->cols * sizeof(int64_t));

    /* initialize row pointers */
    for (uint64_t i = B->row_part.start; i < B->row_part.end; ++i)
    {
        B->m[i] = B->data + (i - B->row_part.start) * B->cols;
    }
    return B;
}


matrix_t* matrix_create (uint64_t rows, uint64_t cols, process_info_t* pinfo)
{
    matrix_t* A;

    /* allocate memory for matrix_t struct */
    A = (matrix_t*) malloc(sizeof(matrix_t));
    if (A == NULL)
    {
        fprintf(stderr, "memory allocation failed\n");
        return NULL;
    }

    /* initialize matrix struct */
    create_partition(&A->row_part, pinfo, rows);
    A->rows = rows;
    A->cols = cols;

    /* allocate memory */
    A->data = calloc(A->row_part.max_len *  cols, sizeof(int64_t));
    A->m = calloc(rows, sizeof(int64_t*));
    if (A->data == NULL || A->m == NULL)
    {
        fprintf(stderr, "memory allocation failed\n");
        free(A);
        return NULL;
    }

    /* initialize row pointers */
    for (uint64_t i = A->row_part.start; i < A->row_part.end; ++i)
    {
        A->m[i] = A->data + (i - A->row_part.start) * cols;
    }
    return A;
}


matrix_t* matrix_read (char* path, process_info_t* pinfo)
{
    matrix_t* A;
    uint64_t rows, cols;
    FILE* file;

    /* allocate memory for matrix_t struct */
    A = (matrix_t*) malloc(sizeof(matrix_t));
    if (A == NULL)
    {
        fprintf(stderr, "memory allocation failed\n");
        return NULL;
    }

    /* open file */
    file = fopen(path, "r");
    if (!file)
    {
        perror("File opening failed\n");
        free(A);
        return NULL;
    }

    /* scan matrix dimensions */
    if (fscanf(file, "%lu %lu\n", &rows, &cols) != 2 || rows == 0 || cols == 0)
    {
        fprintf(stderr, "invalid file\n");
        fclose(file);
        free(A);
        return NULL;
    }

    /* initialize matrix struct */
    A->rows = rows;
    A->cols = cols;
    create_partition(&A->row_part, pinfo, rows);

    /* allocate memory */
    A->data = calloc(A->row_part.max_len * cols, sizeof(int64_t));
    A->m = calloc(rows, sizeof(int64_t*));
    if (A->data == NULL || A->m == NULL)
    {
        fprintf(stderr, "memory allocation failed\n");
        fclose(file);
        free(A);
        return NULL;
    }

    /* initialize row pointer */
    for (uint64_t i = A->row_part.start; i < A->row_part.end; ++i)
    {
        A->m[i] = A->data + (i - A->row_part.start) * cols;
    }

    /* skip offset to local part */
    skip_lines(file, A->row_part.start);

    /* read rows of local part */
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

    /* cleanup */
    fclose(file);
    return A;
}


void matrix_write (char* path, matrix_t* A, process_info_t* pinfo)
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
    else /* pinfo->rank != 0 */
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


void matrix_destroy (matrix_t* A)
{
    free(A->m);
    free(A->data);
    free(A);
}


void matrix_print (matrix_t* A, process_info_t* pinfo)
{
    /* print header */
    if (pinfo->rank == 0)
    {
        printf("[Matrix: %lu x %lu]\n", A->rows, A->cols);
    }
    /* wait for last process */
    if (pinfo->rank > 0)
    {
        MPI_Recv(NULL, 0, MPI_C_BOOL, pinfo->rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
    /* print local part */
    for (uint64_t i = A->row_part.start; i < A->row_part.end; ++i)
    {
        for (uint64_t j = 0; j < A->cols; ++j)
            printf("%ld ", A->m[i][j]);
        printf("\n");
    }
    /* tell next process to go on */
    if (pinfo->rank < pinfo->size - 1)
    {
        MPI_Send(NULL, 0, MPI_C_BOOL, pinfo->rank + 1, 0, MPI_COMM_WORLD);
    }
    MPI_Barrier(MPI_COMM_WORLD);
}
