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
messaggio dividiFrase(char msg[])
{
    messaggio Messaggio;
    char ciao[dim][DIM] = {0};
    int i = 0;
    int k = 0;
    int j = 0;

    memset(&Messaggio, 0, sizeof(Messaggio));

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
    return Messaggio;
}
char confrontaParola(char parola[])
{
    char msg[DIM] = {0};

    if (strncmp("BOOK", parola, 4) == 0)
    {
        strncpy(msg, "OK\n", sizeof(msg));
    }
    else if (strncmp("AVAILABLE", parola, 9) == 0)
    {
        strncpy(msg, "available\n", sizeof(msg));
    }
    else if (strncmp("CANCEL", parola, 6) == 0)
    {
        strncpy(msg, "CANCEL OK\n", sizeof(msg));
    }
    else
    {
        printf("Scrivi la risposta: ");
        fgets(msg, sizeof(msg), stdin);
    }

    return msg[DIM];
}

int main()
{
    char frase[DIM] = {0};
    char msg[DIM] = {0};
    messaggio Struttura;
    memset(&Struttura, 0, sizeof(Struttura));

    printf("Inserisci la frase da dividere: ");
    fgets(frase, sizeof(frase), stdin);

    Struttura = dividiFrase(frase);

    //confronta la parola con le varie possibilità e scrive la risposta nella socket
    strncpy(msg, confrontaParola(Struttura.parola), sizeof(msg));

    printf("la parola scritta è: %s\n", msg);
    /*
    printf("Il numero di argomenti scritti è: %d\n", Struttura.nparole);
    printf("la parola scritta è: %s\n", Struttura.parola);
    printf("Il numero dell'ombrellone è: %d\n", Struttura.ombrellone);
    printf("La fila dell'ombrellone è: %d\n", Struttura.fila);
    printf("La data di inizio è: %d\n", Struttura.data_inizio);
    printf("la data di fine è: %d\n", Struttura.data_fine);
    */
}
