#include<stdio.h>
#include<pthread.h>
#include <unistd.h>

#define N 1000
#define Nt 20

int tab[N];

int extra = 0;

int get_the_smallest(int *tab)
{
    for (int i = 1 + extra; i < N; ++i)
    {
        if (tab[i] == 0)
            continue;
        extra = tab[i];
        return tab[i];
    }
    return 0;
}

void *remove_mod_nr(void *arg)
{
    int nr = *((int *) arg);

    for (int i = 0; i < N; ++i)
    {
        if(tab[i]%nr == 0 && tab[i] !=nr)
        {
            tab[i] = 0;
        }
    }
}

int main()
{
    int nr;
    pthread_t tid[Nt];

    for (int i = 0; i < N; ++i)
    {
        tab[i] = i;
    }

    get_the_smallest(tab);


    for (int i = 0; i < Nt; i++)
    {
        usleep(1000);
        nr = get_the_smallest(tab);
        pthread_create(&tid[i],NULL,remove_mod_nr,(void *) &nr);
    }

    for (int i = 0; i < Nt; ++i)
    {
        pthread_join(tid[i],NULL);
    }

    for (int i = 1, j = 1; i < N; ++i)
    {
        if(tab[i] != 0)
        {
            printf("%d: %d  ", j, tab[i]);
            j++;
            if(!(j % 10))
                printf("\n");
        }


    }
    printf("\n");
    return 0;
}
