#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define DIM 256

typedef struct
{
    char parola[DIM];
    char ombrellone[DIM];
    char fila[DIM];
    char data[DIM];
    char data_fine[DIM];

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
    char ciao[DIM] = {0};

    memset(&Messaggio, 0, sizeof(Messaggio));

    printf("Inserisci frase parola/ombrellone/fila/data/data_fine: ");
    fgets(msg, sizeof(msg), stdin);

    strcpy(Messaggio.parola, strtok(msg, " "));
    strcpy(Messaggio.ombrellone, strtok(NULL, " "));
    strcpy(Messaggio.fila, strtok(NULL, " "));
    strcpy(Messaggio.data, strtok(NULL, " "));
    strcpy(Messaggio.data_fine, strtok(NULL, "\n"));

    int ombrellone = atoi(Messaggio.ombrellone);
    int fila = atoi(Messaggio.fila);
    int data_inizio = uniscidata(Messaggio.data);
    int data_fine = uniscidata(Messaggio.data_fine);

    printf("la parola scritta è: %s\n", Messaggio.parola);
    printf("Il numero dell'ombrellone è: %d\n", ombrellone);
    printf("La fila dell'ombrellone è: %d\n", fila);
    printf("La data di inizio è: %d\n", data_inizio);
    printf("la data di fine è: %d\n", data_fine);
}