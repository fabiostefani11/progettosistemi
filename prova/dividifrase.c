#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define DIM 256
#define dim 10

typedef struct
{
    char parola[DIM];
    char ombrellone_str[DIM];
    char fila_str[DIM];
    char data_inizio_str[DIM];
    char data_fine_str[DIM];
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
    char ciao[dim][DIM];
    int i = 0;
    int j = 0;
    int k = 0;
    while (msg[k] != "\0")
    {
        if (msg[k] == " ")
        {
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

    memset(&Messaggio, 0, sizeof(Messaggio));

    printf("Inserisci frase parola/ombrellone/fila/data/data_fine: ");
    fgets(msg, sizeof(msg), stdin);
}
