#define _GNU_SOURCE

#include<stdlib.h>
#include<stdio.h>
#include<pthread.h>
#include <unistd.h>

int zmienna_wspolna = 0;

#define WYMIAR 1000
#define ROZMIAR WYMIAR*WYMIAR

#define ILOSC 37

double a[ROZMIAR], b[ROZMIAR], c[ROZMIAR];


double czasozajmowacz()
{
    int i, j, k;
    int n = WYMIAR;
    for (i = 0; i < ROZMIAR; i++) a[i] = 1.0 * i;
    for (i = 0; i < ROZMIAR; i++) b[i] = 1.0 * (ROZMIAR - i);
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < n; j++)
        {
            c[i + n * j] = 0.0;
            for (k = 0; k < n; k++)
            {
                c[i + n * j] += a[i + n * k] * b[k + n * j];
            }
        }
    }
    return (c[ROZMIAR - 1]);
}

void *watek(void *arg)
{
    size_t size;
    pthread_attr_t attr;
    pthread_getattr_np(pthread_self(), &attr);
    pthread_attr_getstacksize(&attr, &size);
    printf("%d Rozmiar stosu %d\n", arg, size);
    czasozajmowacz();
    printf("%d - zakonczonono\n", arg);
    return (NULL);
}

int main()
{
    pthread_t tid[ILOSC];
    int wyn[] = {0, 0, 0};
    size_t stack_size = 163840;
    pthread_attr_t attr;
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    for (int j = 2; j < 16; j += 4)
        CPU_SET(j, &cpuset);


    for (int i = 0; i < ILOSC; i++)
    {
        wyn[0] = pthread_attr_init(&attr);
        //wyn[1] = pthread_attr_setstacksize(&attr, stack_size);// ustawianie stosu
        wyn[1] = pthread_attr_setaffinity_np(&attr, sizeof(cpu_set_t), &cpuset);// ustawianie rdzeni


        wyn[2] = pthread_create(&tid[i], &attr, watek, i);

        //printf("%d | %d | %d\n", wyn[0] , wyn[1], wyn[2]);

        pthread_attr_destroy(&attr);
        if (wyn[0] | wyn[1] | wyn[2] != 0)
            printf("ERRROR - %d\n", i);

        //stack_size -= getpagesize()*4
    }
    for (int i = 0; i < ILOSC; i++)
    {
        pthread_join(tid[i], NULL);
    }

    return 0;
}