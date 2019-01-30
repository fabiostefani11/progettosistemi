#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define DIM 256
#define dim 10

typedef struct
{
    char parola[DIM];
    int ID;
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

messaggio dividiFrase(char msg[])
{
    messaggio Messaggio;
    char frase[dim][DIM] = {0};
    int i = 0;
    int k = 0;
    int j = 0;

    memset(&Messaggio, 0, sizeof(Messaggio));

    while (msg[k] != '\n')
    {
        if (msg[k] != ' ')
        {
            frase[j][i] = msg[k];
            i++;
        }
        else if (msg[k] == ' ' && (msg[k + 1] != '\n' && msg[k + 1] != ' '))
        {
            frase[j][i] = '\0';
            j++;
            i = 0;
        }
        k++;
    }

    Messaggio.nparole = j + 1;
    strcpy(Messaggio.parola, frase[0]);
    if (strlen(frase[1]) <= 2)
    {
        Messaggio.fila = atoi(frase[1]);
    }
    else if (strlen(frase[1]) > 2)
    {
        if (frase[1][2] != '/' || frase[1][5] != '/')
        {
            strcpy(Messaggio.parola, "ERRORE_DATA");
            return Messaggio;
        }
        Messaggio.data_inizio = uniscidata(frase[1]);
    }
    if (strlen(frase[2]) <= 2)
    {
        Messaggio.ombrellone = atoi(frase[2]);
        Messaggio.ID = (((Messaggio.fila * 10) + Messaggio.ombrellone) - 10);
    }
    else if (strlen(frase[2]) > 2)
    {
        if (frase[2][2] != '/' || frase[2][5] != '/')
        {
            strcpy(Messaggio.parola, "ERRORE_DATA");
            return Messaggio;
        }
        Messaggio.data_fine = uniscidata(frase[2]);
    }
    if (Messaggio.nparole == 5)
    {
        if (frase[3][2] != '/' || frase[3][5] != '/' || frase[4][2] != '/' || frase[4][5] != '/')
        {
            strcpy(Messaggio.parola, "ERRORE_DATA");
            return Messaggio;
        }
        Messaggio.data_inizio = uniscidata(frase[3]);
        Messaggio.data_fine = uniscidata(frase[4]);
    }
    if (Messaggio.nparole == 4)
    {
        if (frase[3][2] != '/' || frase[3][5] != '/')
        {
            strcpy(Messaggio.parola, "ERRORE_DATA");
            return Messaggio;
        }
        Messaggio.data_fine = uniscidata(frase[3]);
    }
    return Messaggio;
}
int main()
{

    messaggio Messaggio;
    char frase[DIM] = {0};

    printf("Inserisci la frase: ");
    fgets(frase, sizeof(frase), stdin);

    Messaggio = dividiFrase(frase);

    printf("Il numero di argomenti scritti è: %d\n", Messaggio.nparole);
    printf("la parola scritta è: %s\n", Messaggio.parola);
    printf("Il fila dell'ombrellone è: %d\n", Messaggio.fila);
    printf("La numero dell'ombrellone è: %d\n", Messaggio.ombrellone);
    printf("La data di inizio è: %d\n", Messaggio.data_inizio);
    printf("la data di fine è: %d\n", Messaggio.data_fine);
}
