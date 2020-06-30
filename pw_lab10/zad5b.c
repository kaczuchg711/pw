#include <stdio.h>
#include <mpi/mpi.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define number_of_str 500

#define nr_elements_in_buf 40
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
    const int number_of_messages = atoi(argv[1]);

    MPI_Init(&argc, &argv);

    MPI_Datatype mpi_struct;
    int blocklens[3];
    MPI_Aint indices[3];
    MPI_Datatype old_types[3];

    /* One value of each type */
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


    MPI_Comm_size(MPI_COMM_WORLD, &npes);
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);


    MPI_Status status;
    struct Bogata *buffer = (struct Bogata *) malloc(nr_elements_in_buf * number_of_str * sizeof(struct Bogata));
    int bbuf_size, position = 0;
    struct Bogata data[number_of_str];
    struct Bogata bbuf;

    //obliczenie
    int s, buf_size;
    MPI_Pack_size(number_of_str, mpi_struct, MPI_COMM_WORLD, &s);
    buf_size = s + MPI_BSEND_OVERHEAD;

    start = MPI_Wtime();

    switch (myrank)
    {
        case 0:
            MPI_Buffer_attach(buffer, nr_elements_in_buf * buf_size);// buffor dla Bsend
            for (int i = 0; i < number_of_messages; ++i)
            {
                MPI_Pack(data, number_of_str, mpi_struct, buffer, sizeof(struct Bogata) * number_of_str, &position,
                         MPI_COMM_WORLD);
                MPI_Bsend(buffer, position, MPI_PACKED, 1, 100, MPI_COMM_WORLD);

                if (!(i % nr_elements_in_buf))
                {
                    MPI_Buffer_detach(&bbuf, &bbuf_size);
                    MPI_Buffer_attach(buffer, nr_elements_in_buf * buf_size);
                }
                position = 0;
            }
            break;

        case 1:
            for (int i = 0; i < number_of_messages; ++i)
            {
                MPI_Recv(buffer, sizeof(struct Bogata) * number_of_str, MPI_PACKED, 0, 100, MPI_COMM_WORLD, &status);
                MPI_Unpack(buffer, sizeof(struct Bogata) * number_of_str, &position, data, number_of_str, mpi_struct,
                           MPI_COMM_WORLD);
                position = 0;
            }

            break;

    }


    MPI_Finalize();
    end = MPI_Wtime();

    if (myrank == 1)
    {
        char buf[50];
        sprintf(buf, "echo '%d %lf' >>results5b.txt", number_of_messages * number_of_str, end - start);
        system(buf);
    }
    return 0;
}