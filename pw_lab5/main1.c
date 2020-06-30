#include<stdio.h>
#include<pthread.h>

void *merge_sort(void *arg);

void *merge(void *arg);

void merge2(int tab[], int i1, int j1, int i2, int j2);

#define N 8

typedef struct
{
    int *tab;
    int i;
    int j;
} arguments_sort;

typedef struct
{
    int *tab;
    int i1;
    int j1;
    int i2;
    int j2;
} arguments_merge;

int main()
{
    int a[8] = {9, 4, 6, 2, 7, 1, 3, 5};
    arguments_sort arg = {a, 0, 7};

    merge_sort(&arg);

    for (int i = 0; i < N; i++)
        printf("%d ", a[i]);

    printf("\n");

    return 0;
}

void *merge_sort(void *arg)
{
    pthread_t tid[2];
    int mid;

    arguments_sort *wskaznik_do_struktury_main = arg;
    arguments_sort a = *wskaznik_do_struktury_main;

    if (a.i != a.j)
    {
        mid = (a.i + a.j) / 2;
        arguments_sort b = {a.tab, a.i, mid};
        arguments_sort c = {a.tab, mid + 1, a.j};

        pthread_create(&tid[0], NULL, merge_sort, &b);
        pthread_create(&tid[1], NULL, merge_sort, &c);

        pthread_join(tid[0], NULL);
        pthread_join(tid[1], NULL);

        arguments_merge am = {a.tab, a.i, mid, mid + 1, a.j};
        pthread_create(&tid[0], NULL, merge,&am);
        pthread_join(tid[0],NULL);
    }

    return NULL;
}

void *merge(void *arg)
{
    arguments_merge *wskaznik_do_struktury_main = arg;
    arguments_merge am = *wskaznik_do_struktury_main;
    int temp[N];
    int i, j, k;
    i = am.i1;
    j = am.i2;
    k = 0;

    while (i <= am.j1 && j <= am.j2)
    {
        if (am.tab[i] < am.tab[j])
            temp[k++] = am.tab[i++];
        else
            temp[k++] = am.tab[j++];
    }

    while (i <= am.j1)
        temp[k++] = am.tab[i++];

    while (j <= am.j2)
        temp[k++] = am.tab[j++];

    for (i = am.i1, j = 0; i <= am.j2; i++, j++)
        am.tab[i] = temp[j];

    return NULL;
}