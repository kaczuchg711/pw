#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>

//#define _GNU_SOURCE

#include <sys/types.h>
#include <sys/wait.h>
#include <linux/sched.h>
#include <sched.h>
#include<pomiar_czasu.h>

int zmienna_globalna = 0;

#define ROZMIAR_STOSU 1024*64

int funkcja_watku(void *argument)
{

    zmienna_globalna++;

    int wynik;
    wynik = execv("./program", NULL);
    if (wynik == -1)
        printf("Proces potomny nie wykonal programu\n");

    return 0;
}

int main()
{


    void *stos;
    pid_t pid;
    int i;


    stos = malloc(ROZMIAR_STOSU);
    if (stos == 0)
    {
        printf("Proces nadrzędny - blad alokacji stosu\n");
        exit(1);
    }
    double start, stop;
    start = czas_C();

    for (i = 0; i < 1000; i++)
    {

//        potomek       |
//
//        pid = clone(&funkcja_watku, (void *) stos + ROZMIAR_STOSU,CLONE_FS | CLONE_FILES | CLONE_SIGHAND | CLONE_VM, 0);
        pid = clone(&funkcja_watku, (void *) stos + ROZMIAR_STOSU, SIGCHLD | CLONE_VM,0);

        waitpid(pid, NULL, __WCLONE);


    }

    stop = czas_C();
    printf("czas odpalenia 1000 procesow i watkow: %lf\n", stop - start);

    printf("ilosc uruchomien = %d\n", zmienna_globalna);
    free(stos);


    return 0;
}