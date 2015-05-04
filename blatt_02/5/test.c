#include "matrix_mpi.h"
#include "helpers.h"
#include <mpi.h>
#include <stdio.h>


int main (int argc, char** argv)
{
    int ret, rank, size;
    process_info_t pinfo;
    matrix_t* A, * B, * C, * D;

    if ((ret = MPI_Init(&argc, &argv)) != MPI_SUCCESS)
    {
        fprintf(stderr, "Error initializing MPI\n");
        MPI_Abort(MPI_COMM_WORLD, ret);

    }
    
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    create_process_info(&pinfo, size, rank);

    // A = create_matrix(10, 10, &pinfo);
    A = matrix_read("./testmatrix", &pinfo);
    B = matrix_read("./testmatrix", &pinfo);
    matrix_print(A, &pinfo);
    matrix_print(B, &pinfo);

    C = matrix_dot(A, B, &pinfo);

    D = matrix_scalar(A, 2);

    matrix_print(C, &pinfo);
    matrix_print(D, &pinfo);

    matrix_destroy(A);
    matrix_destroy(B);
    matrix_destroy(C);
    // write_matrix("testmatrix", A, &pinfo);

    MPI_Finalize();

    return 0;
}
