#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    int numero;
    int fila;
    int disponibile;
    int giorno_inizio;
    int mese_inizio;
    int anno_inizio;
    int giorno_fine;
    int mese_fine;
    int anno_fine;
} Ombrellone;

int main()
{

    Ombrellone ombrellone[100] = {0};

    int i, n = 1;
    //int A[10];
    int j = 1;

    FILE *f;

    if ((f = fopen("ombrelloni.txt", "w")) == NULL)
    {
        printf("Errore apertura file\n");
        exit(-1);
    }

    for (i = 0; i < 100; i++)
    {

        ombrellone[i].numero = n;
        ombrellone[i].fila = j;
        ombrellone[i].disponibile = 0;
        ombrellone[i].giorno_inizio = 0;
        ombrellone[i].mese_inizio = 0;
        ombrellone[i].anno_inizio = 0;
        ombrellone[i].giorno_fine = 0;
        ombrellone[i].mese_fine = 0;
        ombrellone[i].anno_fine = 0;

        if (n == 10)
        {
            j++;
            n = 0;
        }
        n++;
    }

    for (i = 0; i < 100; i++)
    {
        (fprintf(f, "%d %d %d %d/%d/%d %d/%d/%d \n",
                 ombrellone[i].numero,
                 ombrellone[i].fila,
                 ombrellone[i].disponibile,
                 ombrellone[i].giorno_inizio,
                 ombrellone[i].mese_inizio,
                 ombrellone[i].anno_inizio,
                 ombrellone[i].giorno_fine,
                 ombrellone[i].mese_fine,
                 ombrellone[i].anno_fine));
        if (n == 10)
        {
            j++;
            n = 0;
        }
        n++;
    }

    if (fclose(f))
    {
        printf("Errore chiusura file\n");
        exit(-2);
    }
    return 0;
}