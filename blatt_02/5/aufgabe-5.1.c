#include <mpi.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


typedef struct
{
    int64_t a;
    int64_t b;
    int64_t c;
    char s[20];
} message_t;


int main (int argc, char** argv)
{
    int ret, rank, size;
    message_t m;
    MPI_Datatype mpi_message_t;
    int blocklengths[2];
    MPI_Aint displacements[2];
    MPI_Datatype datatypes[2];
    MPI_Aint m_address, a_address, s_address;

    if ((ret = MPI_Init(&argc, &argv)) != MPI_SUCCESS)
    {
        fprintf(stderr, "Error initializing MPI\n");
        MPI_Abort(MPI_COMM_WORLD, ret);

    }
    
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (size < 2)
    {
        fprintf(stderr, "Two processes required\n");
        MPI_Finalize();
        return EXIT_FAILURE;
    }

    MPI_Get_address(&m, &m_address);
    MPI_Get_address(&m.a, &a_address);
    MPI_Get_address(&m.s, &s_address);
    blocklengths[0] = 3;
    blocklengths[1] = 20;
    displacements[0] = a_address - m_address;
    displacements[1] = s_address - m_address;
    datatypes[0] = MPI_INT64_T;
    datatypes[1] = MPI_CHAR;
    MPI_Type_create_struct(2, blocklengths, displacements, datatypes, &mpi_message_t);
    MPI_Type_commit(&mpi_message_t);

    if (rank == 0)
    {
        m.a = 0;
        m.b = 1;
        m.c = 2;
        strncpy(m.s, "MPI is super fun.", sizeof(m.s));
        m.s[19] = '\0';
        printf("0: %ld, %ld, %ld, %s \n", m.a, m.b, m.c, m.s);
        MPI_Send(&m, 1, mpi_message_t, 1, 0, MPI_COMM_WORLD);
    }
    else if (rank == 1)
    {
        MPI_Recv(&m, 1, mpi_message_t, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("1: %ld, %ld, %ld, %s\n", m.a, m.b, m.c, m.s);
    }

    MPI_Finalize();

    return EXIT_SUCCESS;
}
