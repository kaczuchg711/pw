#include <stdio.h>
#include <mpi/mpi.h>
#include <stdlib.h>

struct Bogata
{
    char name[50];
    char surname[50];
    char data[5000];
};

int main(int argc, char *argv[])
{
    int npes;
    int myrank;
    double start, end;
    int const number_of_messages = atoi(argv[1]);
    MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &npes);
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

    MPI_Datatype mpi_struct;
    int blocklens[3];
    MPI_Aint indices[3];
    MPI_Datatype old_types[3];

    blocklens[0] = 50;
    blocklens[1] = 50;
    blocklens[2] = 5000;


    old_types[0] = MPI_CHAR;
    old_types[1] = MPI_CHAR;
    old_types[2] = MPI_CHAR;


    indices[0] = offsetof(struct Bogata, name);
    indices[1] = offsetof(struct Bogata, surname);
    indices[2] = offsetof(struct Bogata, data);

    MPI_Type_struct(3, blocklens, indices, old_types, &mpi_struct);
    MPI_Type_commit(&mpi_struct);

    struct Bogata data;

    MPI_Status status;

    start = MPI_Wtime();
    if (myrank == 0)
    {
        for (int i = 0; i < number_of_messages; ++i)
        {
            MPI_Send(&data, 1, mpi_struct, 1, 13, MPI_COMM_WORLD);
        }

    } else if (myrank == 1)
    {

        for (int i = 0; i < number_of_messages; ++i)
        {
            MPI_Recv(&data, 1, mpi_struct, 0, 13, MPI_COMM_WORLD, &status);
        }
    }

    end = MPI_Wtime();
    MPI_Finalize();

    if(myrank == 1)
    {
        char buf[50];
        sprintf(buf, "echo '%d %lf' >> results4.txt", number_of_messages, end - start);
        system(buf);
    }
    return 0;
}
