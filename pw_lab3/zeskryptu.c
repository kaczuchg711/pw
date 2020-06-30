#include<stdlib.h>
#include<stdio.h>
#include<pthread.h>
#include <sys/types.h>
#include <unistd.h>
#define _GNU_SOURCE
#include <sched.h>

int zmienna_wspolna=0;

#define WYMIAR 1000
#define ROZMIAR WYMIAR*WYMIAR
double a[ROZMIAR],b[ROZMIAR],c[ROZMIAR];


double czasozajmowacz(){
    int i, j, k;
    int n=WYMIAR;
    for(i=0;i<ROZMIAR;i++) a[i]=1.0*i;
    for(i=0;i<ROZMIAR;i++) b[i]=1.0*(ROZMIAR-i);
    for(i=0;i<n;i++){
        for(j=0;j<n;j++){
            c[i+n*j]=0.0;
            for(k=0;k<n;k++){
                c[i+n*j] += a[i+n*k]*b[k+n*j];
            }
        }
    }
    return(c[ROZMIAR-1]);
}

void * zadanie_watku (void * arg_wsk)
{
    pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
    printf("\twatek potomny: uniemozliwione zabicie\n");

    czasozajmowacz();

    printf("\twatek potomny: umozliwienie zabicia\n");
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);

    pthread_testcancel();

    zmienna_wspolna++;
    printf("\twatek potomny: zmiana wartosci zmiennej wspolnej\n");

    return(NULL);
}

int main()
{
    cpu_set_t cpuset;
    pthread_t tid;
    pthread_attr_t attr;
    size_t stksize;
    void *wynik;
    pthread_t thread;
    int i;

    pthread_attr_init(&attr);

//  wielkosc stosu
//  pthread_attr_setstacksize(&attr, STOS);



    CPU_ZERO(&cpuset);
    for(i = 0; i<3; i++)
    {
        CPU_SET(i, &cpuset);
    }
    pthread_setaffinity_np(tid, sizeof(cpu_set_t), &cpuset);

    pthread_create(&tid, &attr, zadanie_watku, NULL);

    sleep(2); // czas na uruchomienie watku

    printf("\twatek glowny: wyslanie sygnalu zabicia watku\n");
    pthread_cancel(tid);

    pthread_join(tid, &wynik);

    if (wynik == PTHREAD_CANCELED)
        printf("\twatek glowny: watek potomny zostal zabity\n");
    else
        printf("\twatek glowny: watek potomny NIE zostal zabity - blad\n");

    //exit(0);
    pthread_exit(NULL); // co stanie sie gdy uzyjemy exit(0)?
}