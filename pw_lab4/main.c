#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>


#define LB_KUFLI 5
#define LB_Klienci 10
#define max_czas_picia_piwa 5

//zmiana kufli trzeba dopisać
static pthread_mutex_t kufle[LB_KUFLI] = {
        PTHREAD_MUTEX_INITIALIZER,
        PTHREAD_MUTEX_INITIALIZER,
        PTHREAD_MUTEX_INITIALIZER,
        PTHREAD_MUTEX_INITIALIZER,
        PTHREAD_MUTEX_INITIALIZER

};

void *picie(void *arg)
{

    int czas = rand() % max_czas_picia_piwa;
    int wynik;
    int nr_kufla;

//    dwa razy chce się napić
    for (int i = 0; i < 2; ++i)
    {
//      wybiera dopóki nie trafi na wolny
        while (1)
        {
            nr_kufla = rand() % LB_KUFLI;
            wynik = pthread_mutex_trylock(&kufle[nr_kufla]);
            if (wynik != 16)
            {
                break;
            }
        }

        printf("Klient nr %d zamawia piwo w kuflu nr %d\n", *((int *) arg), nr_kufla);
        sleep(czas);
        printf("Klient nr %d \t oddaje kufel nr %d po czasie %d\n", *((int *) arg), nr_kufla, czas);
        pthread_mutex_unlock(&kufle[nr_kufla]);
    }
}


int main()
{
    pthread_t klienci[LB_Klienci];

    for (int i = 0; i < LB_Klienci; i++)
    {
//      za każdym razem zaalokuje wartość w innym miejscu i nie będzie głupot po przesłaniu argumentu
        int *arg = malloc(sizeof(*arg));
        *arg = i;

        pthread_create(&klienci[i], NULL, picie, (void *) arg);
    }

    for (int j = 0; j < LB_Klienci; j++)
    {
        pthread_join(klienci[j], NULL);
    }

    return 0;
}