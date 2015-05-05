matrix_dot (matrix A, matrix B):

assert A.cols == B.rows
matrix C <- new matrix (A.rows x B.cols) filled with zeros

for i = A.row_local_start to A.row_local_end do
    for k = B.row_local_start to B.row_local_end do
        for j = 0 to B.cols do
            C[i,j] += A[i,k] * B[k,j]

// copy local part of B
T <- copy of B

for number_of_processes - 1 times do
    // shift local parts of B
    send T.local_data to next rank and
    replace T.local_data with data from previous rank
    send local metadata of T to next rank and
    replace local metadata of T with data from previous rank

    for i = A.row_local_start to A.row_local_end do
        for k = T.row_local_start to T.row_local_end do
            for j = 0 to B.cols do
                C[i,j] += A[i,k] * T[k,j]
end for
    
return C
