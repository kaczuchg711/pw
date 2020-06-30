#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wimplicit-function-declaration"
#define len 100000000

void fill_vec(double *vector)
{
    for (int j = 0; j < len; ++j)
    {
        vector[j] = rand();
    }
}

void vector_multiplication(double *vector1, double *vector2, double *res)
{
    double start, end;
//zad3
    start = omp_get_wtime();
    for (int i = 0; i < len; ++i)
    {
        res[i] = vector1[i] * vector2[i];
    }
    end = omp_get_wtime();
    printf("1 bez zrownoleglenia:\n %lf \n", end - start);

//zad4

    start = omp_get_wtime();
#pragma omp parallel for num_threads(4) schedule(dynamic,len/4)

    for (int j = 0; j < len; ++j)
    {
        res[j] = vector1[j] * vector2[j];
    }
    end = omp_get_wtime();
    printf("2 z zrownolegleniem:\n %lf\n", end - start);

    start = omp_get_wtime();
#pragma simd omp

    for (int j = 0; j < len; ++j)
    {
        res[j] = vector1[j] * vector2[j];
    }
    end = omp_get_wtime();
    printf("3 z simd:\n %lf\n", end - start);
}

int main()
{
    srand(time(NULL));
    double *vector1;
    double *vector2;
    double *res;


    vector1 = malloc(len * sizeof(double));
    vector2 = malloc(len * sizeof(double));
    res = malloc(len * sizeof(double));

    fill_vec(vector1);
    fill_vec(vector2);

    vector_multiplication(vector1, vector2, res);

    free(vector1);
    free(vector2);
    free(res);
    return 0;
}
