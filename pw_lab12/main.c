#include <mpi/mpi.h>
#include "pbm.h"
#include <stdio.h>


int filterTable[3][3] = {{0,  -1, 0},
                         {-1, 5,  -1},
                         {0,  -1, 0}};


uchar filter(uchar *pixels, int i, int with, int j)
{
    int suma = 0;

    for (int k = -1; k < 2; ++k)
        for (int l = -1; l < 2; ++l)
            suma += (int) pixels[(i + k) * with + (j + l)] * filterTable[k + 1][l + 1];

    int denominator = 0;
    for (int k = 0; k < 3; ++k)
        for (int l = 0; l < 3; ++l)
            denominator += filterTable[k][l];


    if (suma > 255) suma = 255;
    else if (suma < 0) suma = 0;

    return suma / denominator;
}

int main(int argc, char *argv[])
{
    image in;
    image out;
    image outpart;
    image inpart;
    int pwith;
    int pheight;

    int npes;
    int myrank;
    int root = 0;
    double start, end;


    MPI_Status status;
    MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &npes);
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);


    int Bcastbuf[2];

    if (myrank == 0)
    {
        readInput(argv[1], &in);
        out.pixel = (uchar *) malloc(sizeof(uchar) * in.height * in.width);
        out.height = in.height;
        out.width = in.width;
        out.maxValue = in.maxValue;
        memcpy(out.type, in.type, TYPE_LEN + 1);

        pheight = in.height / npes;
        pwith = in.width;

        Bcastbuf[0] = pheight;
        Bcastbuf[1] = pwith;
    }

    MPI_Bcast(Bcastbuf, 2, MPI_INT, root, MPI_COMM_WORLD);

    pheight = Bcastbuf[0];
    pwith = Bcastbuf[1];

    inpart.pixel  = (uchar *) malloc(sizeof(uchar) * pwith * pheight);
    outpart.pixel = (uchar *) malloc(sizeof(uchar) * pwith * pheight);

    MPI_Scatter(in.pixel, pheight * pwith, MPI_CHAR, inpart.pixel,pheight * pwith, MPI_INT, 0, MPI_COMM_WORLD);

    start = MPI_Wtime();

    for (int i = 1; i < pheight - 1; ++i)
        for (int j = 1; j < pwith - 1; ++j)
            outpart.pixel[i * pwith + j] = filter(inpart.pixel, i, pwith, j);

    end = MPI_Wtime();

    MPI_Gather(outpart.pixel, pheight * pwith, MPI_CHAR, out.pixel,
            pheight * pwith, MPI_CHAR, 0, MPI_COMM_WORLD);

    if (myrank == 0)
    {
        int x = pheight -1;
        for (int i = 0; i < npes; ++i)
        {
            for (int j = 1; j < pwith - 1; ++j)
            {
                out.pixel[x * pwith + j] = filter(in.pixel, x, pwith, j);
                out.pixel[(x + 1) * pwith + j] = filter(in.pixel, x + 1, pwith, j);
            }
            x += pheight;
        }
        writeData(argv[2], &out);
        printf("%lf\n",end - start);
    }
    free(inpart.pixel);
    free(outpart.pixel);

    MPI_Finalize();

    return 0;

}
