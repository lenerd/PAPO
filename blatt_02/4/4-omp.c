matrix_dot (matrix A, matrix B):

assert A.cols == B.rows
matrix C <- new matrix (A.rows x B.cols) filled with zeros

for i = 0 to A.rows parallel do
    for j = 0 to B.cols do
        for k = 0 to A.cols do
            C[i,j] += A[i,k] * B[k,j]

return C
