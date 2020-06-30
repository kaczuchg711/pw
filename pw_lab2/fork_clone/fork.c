#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<sys/wait.h>
#include<pomiar_czasu.h>

int zmienna_globalna=0;

int main()
{
    int pid, wynik, i;
    double start,stop;
    start = czas_C();

    for(i=0;i<1000;i++){

        pid = fork();
        if(pid==0)
        {
             wynik=execv("./program",NULL);
             if(wynik==-1)
                 printf("Proces potomny nie wykonal programu\n");

            exit(0);

        } else {
            zmienna_globalna++;

            wait(NULL);

        }
    }

    stop = czas_C();

    printf("czas uruchomienia 1000 procesow i watkow: %lf\nliczba procesow: %d\n",stop - start,zmienna_globalna);
    return 0;
}


