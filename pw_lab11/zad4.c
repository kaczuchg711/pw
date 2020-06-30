#include <stdio.h>
#include <stdlib.h>
#include "mpi/mpi.h"
#include "math.h"


#define p 10


double sum_for_Euler_gamma(int n, int m)
{
    double sum = 0;
    for (int i = n; i <= m; ++i)
    {
        sum += 1. / i;
    }

    return sum;
}


int main(int argc, char *argv[])
{

    int myrank, nr_process;

    int n = 0;
    int BcastBuffer[2];
    int root = 0;

    double start, end;

    int remainder = 0;
    MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &nr_process);
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);


//    wczytywania z klawiatury i przesyłanie do innych procesow
    start = MPI_Wtime();
    int elements_per_process = 0;


    if (!myrank)
    {
        scanf("%d", &n);

        elements_per_process = n / p;

        if (n % p != 0)
            remainder = n % p;

        BcastBuffer[0] = elements_per_process;
        BcastBuffer[1] = remainder;
    }


    MPI_Bcast(BcastBuffer, 2, MPI_INT, root, MPI_COMM_WORLD);

//
    double localsum;
    double globalsum;
    localsum = sum_for_Euler_gamma(1 + (BcastBuffer[0] * (myrank)), (myrank + 1) * BcastBuffer[0]);

    MPI_Reduce(&localsum, &globalsum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    end = MPI_Wtime();

    if (!myrank)
    {
        //      obliczanie reszty
        if (remainder)
            localsum = sum_for_Euler_gamma(p * elements_per_process + 1,(p * elements_per_process) + remainder);
        else
            localsum = 0;
        printf("Stałą gamma Eulera : %lf\n", ((globalsum + localsum) - log(n)));
        printf("czas: %lf\n", end - start);
    }

    MPI_Finalize();

    return 0;
}
