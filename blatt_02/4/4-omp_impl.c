matrix_t* AB (matrix_t* A, matrix_t* B)
{
    assert(A->cols == B->rows);
    matrix_t* C = create_matrix(A->rows, B->cols);
    uint64_t i, j, k;
    #pragma omp parallel for private(i, j, k)
    for (i = 0; i < A->rows; ++i)
        for (j = 0; j < B->cols; ++j)
            for (k = 0; k < A->cols; ++k)
                C->m[i][j] += A->m[i][k] * B->m[k][j];
    return C;
}
