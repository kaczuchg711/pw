#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <time.h>

void matrix_multiplication(const int *A, const int *B, int *C, int n, int m, int p)
{
//    n - wiersze A
//    m - kolumny A wiersze B
//    p - kolumny B
    int sum = 0;
#pragma omp parallel for num_threads(4) firstprivate(sum)
    for (int i = 0; i < n; ++i)
    {
#pragma omp parallel for firstprivate(i,sum) num_threads(4)
        for (int k = 0; k < p; ++k)
        {

            for (int j = 0; j < m; ++j)
            {
                sum += *(B + k + (p * j)) * *(A + j + ((n + 1) * i));
            }
            *(C + k + ((n + 1) * (i) + i)) = sum;
            sum = 0;
        }
    }
}

void gen_matrix(int n, int m, char *name)
{
    int num;
    FILE *fptr;
    fptr = fopen(name, "w+");
    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < m; ++j)
        {
            num = rand() % 100;
            fprintf(fptr, "%d ", num);
        }
        fprintf(fptr, "\n");
    }
    fclose(fptr);
}

void init_matrix(int n, int m, int *matrix, char *name)
{
    FILE *fptr;
    fptr = fopen(name, "r");
    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < m; ++j)
        {
            fscanf(fptr, "%d", matrix + j + ((m) * i));
        }
    }
    fclose(fptr);
}

void save_res(int n, int m, const int *matrix, char *name)
{
    FILE *fptr;
    fptr = fopen(name, "w");
    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < m; ++j)
        {
            fprintf(fptr, "%d ", *(matrix + j + ((m) * i)));
        }
        fprintf(fptr, "\n");
    }
    fclose(fptr);
}

int main()
{
    printf("zad4 zewnetrzna wewnetrzna\n");
    printf("mala     srednia  duza\n");
    double start, end;
    for (int i = 0; i < 5; ++i)
    {


//    liczba wierszy A = kolumny B
//    n - wiersze A C
//    m - kolumny A wiersze B
//    p - kolumny B C
//     A[n][m];
//     B[m][p];
//     C[n][p];
//

        srand((unsigned int) time(NULL));
//
#define n1 3
#define m1 4
#define p1 5

        int A[n1][m1];
//    gen_matrix(n1, m1, "matrices/A");
        init_matrix(n1, m1, (int *) A, "matrices/A");

        int B[m1][p1];
//    gen_matrix(m1, p1, "matrices/B");
        init_matrix(m1, p1, (int *) B, "matrices/B");

        int C[n1][p1];
        start = omp_get_wtime();
        matrix_multiplication((int *) A, (int *) B, (int *) C, n1, m1, p1);
        end = omp_get_wtime();
        save_res(n1, p1, (int *) C, "matrices/C");


        printf("%f ", end - start);

#define n2 100
#define m2 100
#define p2 100

        int D[n2][m2];
//    gen_matrix(n2, m2, "matrices/D");
        init_matrix(n2, m2, (int *) A, "matrices/D");

        int E[m2][p2];
//    gen_matrix(m2, p2, "matrices/E");
        init_matrix(m2, p2, (int *) B, "matrices/E");

        int F[n2][p2];
        start = omp_get_wtime();
        matrix_multiplication((int *) D, (int *) E, (int *) F, n2, m2, p2);
        end = omp_get_wtime();

        printf("%f ", end - start);

#define n3 600
#define m3 600
#define p3 600

        int G[n3][m3];
//        gen_matrix(n3, m3, "matrices/G");
        init_matrix(n3, m3, (int *) G, "matrices/G");

        int H[m3][p3];
//        gen_matrix(m3, p3, "matrices/H");
        init_matrix(m3, p3, (int *) H, "matrices/H");

        int J[n3][p3];
        start = omp_get_wtime();
        matrix_multiplication((int *) G, (int *) H, (int *) J, n3, m3, p3);
        end = omp_get_wtime();

        printf("%f\n", end - start);

    }
    return 0;
}