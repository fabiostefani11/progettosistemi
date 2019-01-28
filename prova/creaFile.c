#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    int ID;
    int numero;
    int fila;
    int disponibile;

} Ombrellone;

int main()
{

    Ombrellone ombrellone[100] = {0};

    int i, n = 1;
    int j = 1;

    FILE *f;

    if ((f = fopen("ombrelloni.txt", "w")) == NULL)
    {
        printf("Errore apertura file\n");
        exit(-1);
    }

    for (i = 0; i < 100; i++)
    {
        ombrellone[i].ID = i + 1;
        ombrellone[i].numero = n;
        ombrellone[i].fila = j;
        ombrellone[i].disponibile = 1;

        if (n == 10)
        {
            j++;
            n = 0;
        }
        n++;
    }

    for (i = 0; i < 100; i++)
    {
        (fprintf(f, "%d %d %d %d \n",
                 ombrellone[i].ID,
                 ombrellone[i].numero,
                 ombrellone[i].fila,
                 ombrellone[i].disponibile));

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