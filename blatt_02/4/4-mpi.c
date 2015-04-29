#include <assert.h>
#include <mpi.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
 * Struct representing a range.
 */
typedef struct 
{
    uint64_t start; /* inclusive */
    uint64_t end;   /* exclusive */
    uint64_t len;
} partition_t;

/**
 * Struct with process information.
 */
typedef struct 
{
    uint64_t P;     /* number of processes */
    uint64_t rank;  /* rank */
} proc_info_t;


/**
 * Creates proc_info_t struct.
 */
void create_proc_info (proc_info_t* pinfo, uint64_t p, uint64_t r);

/**
 * create partition_t struct.
 */
void create_partition (partition_t* part, uint64_t n, uint64_t p, uint64_t r);

/**
 * Calculates matrix vector product A*x.
 */
vector_t* Ax (matrix_t* A, vector_t* x);

/**
 * Calculates matrix product A*B.
 */
matrix_t* AB (matrix_t* A, matrix_t* B, proc_info_t* pinfo);

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


/**
 * Reads two matrices A, B from files (first two parameters).
 * Calculates A*B and writes result to file (parameter three).
 */
int main (int argc, char** argv)
{
    int P, rank, ret;
    matrix_t* A, * B, * C;
    proc_info_t pinfo;
    partition_t part;

    if (argc < 4)
        return EXIT_FAILURE;

    if ((ret = MPI_Init(&argc, &argv)) != MPI_SUCCESS)
    {
        printf("Error initializing MPI\n");
        MPI_Abort(MPI_COMM_WORLD, ret);
    }

    MPI_Comm_size(MPI_COMM_WORLD, &P);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    create_proc_info (&pinfo, (uint64_t)P, (uint64_t)rank);

    /* read matrix A */
    A = read_matrix(argv[1]);
    if (A == NULL)
        return EXIT_FAILURE;

    /* read matrix B */
    B = read_matrix(argv[1]);
    if (B == NULL)
        return EXIT_FAILURE;

    /* calc C = A*B */
    C = AB(A, B, &pinfo);

    /* get all parts of C to root */
    if (rank == 0) /* recv parts */
    {
        for (int r = 0; r < P; ++r)
        {
            create_partition (&part, C->rows, (uint64_t)P, (uint64_t)r);
            MPI_Recv(C->m[part.start], (int) (part.len * C->cols), MPI_INT64_T, r, r, MPI_COMM_WORLD, NULL);
        }
    }
    else /* send parts */
    {
        create_partition (&part, C->rows, (uint64_t)P, (uint64_t)rank);
        MPI_Send(C->m[part.start], (int)(part.len * C->cols), MPI_INT64_T, 0, rank, MPI_COMM_WORLD);
    }

    /* write C */
    if (rank == 0)
        write_matrix(argv[3], C);

    /* cleanup */
    destroy_matrix(A);
    destroy_matrix(B);
    destroy_matrix(C);
    MPI_Finalize();
    return EXIT_SUCCESS;
}


void create_proc_info (proc_info_t* pinfo, uint64_t P, uint64_t rank)
{
    pinfo->P = P;
    pinfo->rank = rank;
}


void create_partition (partition_t* part, uint64_t n, uint64_t p, uint64_t r)
{
    uint64_t base_len, rest;
    base_len = n / p;
    rest = n % p;
    part->start = r * base_len;
    part->start += r < rest ? r : rest;
    part->end = (r + 1) * base_len;
    part->end += (r + 1) < rest ? (r + 1) : rest;
    part->len = part->end - part->start;
}


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


matrix_t* AB (matrix_t* A, matrix_t* B, proc_info_t* pinfo)
{
    assert(A->cols == B->rows);
    matrix_t* C = create_matrix(A->rows, B->cols);
    uint64_t i, j, k;
    partition_t part;
    /* split rows of A,C */
    create_partition(&part, A->rows, pinfo->P, pinfo->rank);
    for (i = part.start; i < part.end; ++i)
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
    A->m = calloc(rows, sizeof(int64_t*));
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
