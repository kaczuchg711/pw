#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

long int fibbonacci1(int n)
{
    if (n == 0) return 0;
    else if (n == 1) return 1;
    else return (fibbonacci1(n - 1) + fibbonacci1(n - 2));
}

long int fibbonacci2(int n)
{
    if (n == 0) return 0;
    else if (n == 1) return 1;
    else
    {
        int i;
        int j;
#pragma omp task shared(i)
        i = fibbonacci2(n - 1);
#pragma omp task shared(j)
        j = fibbonacci2(n - 2);
#pragma omp taskwait
        return i + j;
    }
}

long int fibbonacci3(int n)
{
    unsigned long long i, j;
#pragma omp parallel
    {
#pragma omp single
        {
#pragma omp task shared(i)
            if (n < 38)
                i = fibbonacci1(n - 1);
            else
                i = fibbonacci3(n - 1);
#pragma omp task shared(j)
            if (n < 38)
                j = fibbonacci1(n - 2);
            else
                j = fibbonacci3(n - 2);

        }
    }
    return i + j;
}


int main()
{
    double start, end;
    long int res;

    start = omp_get_wtime();
    res = fibbonacci1(42);
    end = omp_get_wtime();
    printf("1: res = %ld\t %f\n", res, end - start);

    start = omp_get_wtime();
    res = fibbonacci2(42);
    end = omp_get_wtime();

    printf("2: res = %ld\t %f\n", res, end - start);

    start = omp_get_wtime();
    res = fibbonacci3(42);
    end = omp_get_wtime();

    printf("3: res = %ld\t %f\n", res, end - start);


    return 0;
}
