#include <stdio.h>
#include <omp.h>
#include <zconf.h>
#include <stdlib.h>
#include <string.h>

#define NRi 50

void f()
{
    double res[4][100];
    int a = 7;
    double start, end;

    for (int j = 0; j < 100; j++)
    {
//        printf("schedule(static,3)\n");
        start = omp_get_wtime();
#pragma omp parallel for firstprivate(a) num_threads(7) schedule(static, 3)

        for (int i = 0; i < NRi; i++)
        {
//        printf("\tThread %d a=%d\n", omp_get_thread_num(), a);
            a++;
        }
        end = omp_get_wtime();
//        printf("\tresult: %d, czas: %f\n", a, end - start);
        res[0][j] = end - start;

//        printf("schedule(static)\n");
        start = omp_get_wtime();
#pragma omp parallel for firstprivate(a) num_threads(7) schedule(static)

        for (int i = 0; i < NRi; i++)
        {
//        printf("\tThread %d a=%d\n", omp_get_thread_num(), a);
            a++;
        }
        end = omp_get_wtime();
//        printf("\tresult: %d, czas: %f\n", a, end - start);
        res[1][j] = end - start;

//        printf("schedule(dynamic,3)\n");
        start = omp_get_wtime();
#pragma omp parallel for firstprivate(a) num_threads(7) schedule(dynamic, 3)

        for (int i = 0; i < NRi; i++)
        {
//        printf("\tThread %d a=%d\n", omp_get_thread_num(), a);
            a++;
        }
        end = omp_get_wtime();
//        printf("\tresult: %d, czas: %f\n", a, end - start);
        res[2][j] = end - start;

//        printf("schedule(dynamic)\n");
        start = omp_get_wtime();
#pragma omp parallel for firstprivate(a) num_threads(7) schedule(dynamic)

        for (int i = 0; i < NRi; i++)
        {
//        printf("\tThread %d a=%d\n", omp_get_thread_num(), a);
            a++;
        }
        end = omp_get_wtime();
//        printf("\tresult: %d, czas: %f\n", a, end - start);
        res[3][j] = end - start;
    }
    double suma = 0;
    for (int k = 0; k < 4; k++)
    {
        for (int i = 0; i < 100; i++)
        {
            suma += res[k][i];
        }
        res[k][0] = suma / 100;
    }
    printf("(static,3), (static), (dynamic,3), (dynamic)\n");
    printf("%10f, %8f, %11f, %9f \n", res[0][0], res[1][0], res[2][0], res[3][0]);
}

int main()
{
    int a = 2;
    int suma = 0;
    double start, end;


    start = omp_get_wtime();
#pragma omp parallel for schedule(static, 3) num_threads(4) shared(a)

    for (int i = 0; i < 500; i++)
    {
#pragma omp critical
        {
            suma += a * a;
        }
    }
    end = omp_get_wtime();
    printf("critical\n");
    printf("suma: %d,czas: %f\n", suma, end - start);

    suma = 0;
    start = omp_get_wtime();
#pragma omp parallel for schedule(static, 3) num_threads(4) shared(a)     reduction( + : suma)

    for (int i = 0; i < 500; i++)
    {
        suma += a * a;
    }
    end = omp_get_wtime();
    printf("reduction\n");
    printf("suma: %d,czas: %f\n", suma, end - start);

    suma = 0;
    start = omp_get_wtime();

    omp_lock_t lock;

    omp_init_lock(&lock);

#pragma omp parallel for schedule(static, 3) num_threads(4) shared(a)     reduction( + : suma)


    for (int i = 0; i < 500; i++)
    {
        omp_set_lock(&lock);
        suma += a * a;
        omp_unset_lock(&lock);
    }
    end = omp_get_wtime();
    printf("lock\n");
    printf("suma: %d,czas: %f\n", suma, end - start);
    omp_destroy_lock(&lock);

    suma = 0;

    start = omp_get_wtime();
#pragma omp parallel for schedule(static, 3) num_threads(4) shared(a)

    for (int i = 0; i < 500; i++)
    {
        #pragma omp atomic
        suma += a * a;
    }
    end = omp_get_wtime();
    printf("atomic\n");
    printf("suma: %d,czas: %f\n", suma, end - start);

    suma = 0;


    return 0;
}

