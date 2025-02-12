#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <omp.h>

#include "search_max_openmp.h"

#define WYMIAR 1000000
#define ROZMIAR WYMIAR


int main()
{
    static double a[ROZMIAR];
    int n, i, j;
    double a_max;
    double t1;
    double suma = 0;

    for (i = 0; i < ROZMIAR; i++) a[i] = (0.00000001 * i) * (ROZMIAR - 1 - i);

    n = ROZMIAR;

    suma = 0;
    for (int k = 0; k < 1000; ++k)
    {
        t1 = omp_get_wtime();
        a_max = search_max(a, 0, n - 1);
        t1 = omp_get_wtime() - t1;
        suma += t1;
    }
    printf("maximal element %lf\n", a_max);
    printf("time for sequential linear search: %lf\n", suma/1000);


    suma = 0;
    for (int k = 0; k < 1000; ++k)
    {
        t1 = omp_get_wtime();
        a_max = search_max_openmp_simple(a, 0, n - 1);
        t1 = omp_get_wtime() - t1;
        suma += t1;
    }

    printf("maximal element %lf\n", a_max);
    printf("time for parallel linear search: %lf\n", suma / 1000);

    suma = 0;
    for (int k = 0; k < 1000; ++k)
    {
        t1 = omp_get_wtime();
        a_max = search_max_openmp_task(a, 0, n - 1);
        t1 = omp_get_wtime() - t1;
        suma += t1;
    }
    printf("maximal element %lf\n", a_max);
    printf("\t\t\ttime for parallel linear search: %lf\n", suma / 1000);

    suma = 0;
    for (int k = 0; k < 1000; ++k)
    {
        t1 = omp_get_wtime();
        a_max = bin_search_max(a, 0, n - 1);
        t1 = omp_get_wtime() - t1;
        suma += t1;
    }
    printf("maximal element %lf\n", a_max);
    printf("time for sequential binary search: %lf\n", suma/t1);

    suma = 0;
    for (int k = 0; k < 1000; ++k)
    {
        t1 = omp_get_wtime();
        a_max = bin_search_max_openmp(a, 0, n - 1);
        t1 = omp_get_wtime() - t1;
        suma += t1;
    }
    printf("maximal element %lf\n", a_max);
    printf("time for parallel binary search: %lf\n", suma/1000);

    return 0;
}


