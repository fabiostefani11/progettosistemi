#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define DIM 256
#define dim 10

typedef struct
{
    char parola[DIM];
    int ombrellone;
    int fila;
    int data_inizio;
    int data_fine;
    int nparole;

} messaggio;

int uniscidata(char data[])
{
    char giorno[DIM] = {0};
    char mese[DIM] = {0};
    char anno[DIM] = {0};

    strcpy(giorno, strtok(data, "/"));
    strcpy(mese, strtok(NULL, "/"));
    strcpy(anno, strtok(NULL, "/"));

    strcat(anno, mese);
    strcat(anno, giorno);

    return atoi(anno);
}
int main()
{
    messaggio Messaggio;
    char msg[DIM] = {0};
    char ciao[dim][DIM] = {0};
    int i = 0;
    int k = 0;
    int j = 0;

    memset(&Messaggio, 0, sizeof(Messaggio));

    printf("Inserisci la frase da dividere: ");
    fgets(msg, sizeof(msg), stdin);

    while (msg[k] != '\n')
    {
        if (msg[k] == ' ')
        {
            ciao[j][i] = '\0';
            k++;
            j++;
            i = 0;
        }
        else
        {
            ciao[j][i] = msg[k];
            i++;
            k++;
        }
    }
    Messaggio.nparole = j + 1;
    strcpy(Messaggio.parola, ciao[0]);
    if (strlen(ciao[1]) <= 2)
    {
        Messaggio.ombrellone = atoi(ciao[1]);
    }
    else if (strlen(ciao[1]) > 2)
    {
        Messaggio.data_inizio = uniscidata(ciao[1]);
    }
    if (strlen(ciao[2]) <= 2)
    {
        Messaggio.fila = atoi(ciao[2]);
    }
    else if (strlen(ciao[2]) > 2)
    {
        Messaggio.data_fine = uniscidata(ciao[2]);
    }
    if (Messaggio.nparole > 3)
    {
        Messaggio.data_inizio = uniscidata(ciao[3]);
        Messaggio.data_fine = uniscidata(ciao[4]);
    }

    printf("Il numero di argomenti scritti è: %d\n", Messaggio.nparole);
    printf("la parola scritta è: %s\n", Messaggio.parola);
    printf("Il numero dell'ombrellone è: %d\n", Messaggio.ombrellone);
    printf("La fila dell'ombrellone è: %d\n", Messaggio.fila);
    printf("La data di inizio è: %d\n", Messaggio.data_inizio);
    printf("la data di fine è: %d\n", Messaggio.data_fine);
}