#include <stdio.h>
#include <mpi/mpi.h>
#include <stdlib.h>

double leibniz_series(int n, int m)
{
    double sum = 0;

    for (int i = n; i <= m; ++i)
        sum += (i % 2 == 0 ? -1.0 : 1.0) / (2 * i - 1);

    return sum;
}

int main(int argc, char *argv[])
{

    int const nr_elements = atoi(argv[1]);

    int nr_process;
    int myrank;
    double start, end;


    MPI_Init(&argc, &argv);

    start = MPI_Wtime();
    MPI_Comm_size(MPI_COMM_WORLD, &nr_process);
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);


// gdy liczba elementow jest mala uruchamiana zostaje metoda na jednm procesie
    if (nr_elements < nr_process)
    {
        MPI_Finalize();
        if (!myrank)
            printf("%lf\n", leibniz_series(1, nr_elements) * 4);
        return 0;
    }


    int elements_per_process;
    int remainder = 0;
    int BcastBuffer[2];
    int root = 0;

    if(!myrank)
    {
        elements_per_process = nr_elements / nr_process;

        if (nr_elements / nr_process % nr_process != 0)
            remainder = nr_elements % nr_process;

        BcastBuffer[0] = elements_per_process;
        BcastBuffer[1] = remainder;
    }

    MPI_Bcast(BcastBuffer, 2, MPI_INT, root, MPI_COMM_WORLD);



    double localsum = 0;
    double globalsum = 0;


// obsluga reszty ostatni z watkow dodaje reszte
    if (remainder)
    {
        if (myrank != nr_process - 1)
            localsum = leibniz_series(1 + (BcastBuffer[0] * (myrank)), (myrank + 1) * BcastBuffer[0]);
        else
            localsum = leibniz_series(1 + (BcastBuffer[0] * (myrank)),
                                      (myrank + 1 + BcastBuffer[1] ) * BcastBuffer[0]);
    }
    else
        localsum = leibniz_series(1 + (BcastBuffer[0] * (myrank)), (myrank + 1) * BcastBuffer[0]);


    MPI_Reduce(&localsum, &globalsum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    end = MPI_Wtime();

    if (!myrank)
    {
        // tu mnozone jest dopiero *4 aby bylo mniej dzialan
        printf("PI %lf\n", globalsum * 4);
        printf("end of process %lf\n", end - start);
    }

    MPI_Finalize();
    return 0;
}
