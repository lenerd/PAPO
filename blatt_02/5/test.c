#include "matrix_mpi.h"
#include "helpers.h"
#include <mpi.h>
#include <stdio.h>


int main (int argc, char** argv)
{
    int ret, rank, size;
    process_info_t pinfo;
    matrix_t* A;

    if ((ret = MPI_Init(&argc, &argv)) != MPI_SUCCESS)
    {
        fprintf(stderr, "Error initializing MPI\n");
        MPI_Abort(MPI_COMM_WORLD, ret);

    }
    
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    create_process_info(&pinfo, size, rank);

    // A = create_matrix(10, 10, &pinfo);
    A = read_matrix("./testmatrix", &pinfo);
    print_matrix(A, &pinfo);
    // write_matrix("testmatrix", A, &pinfo);

    MPI_Finalize();

    return 0;
}
