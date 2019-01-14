#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define DIM 256

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

    memset(&Messaggio, 0, sizeof(Messaggio));

    printf("Inserisci frase parola/ombrellone/fila/data/data_fine: ");
    fgets(msg, sizeof(msg), stdin);

    strcpy(Messaggio.parola, strtok(msg, " \n"));
     printf("la parola scritta è: %s\n", Messaggio.parola);
    strcpy(Messaggio.ombrellone_str, strtok(NULL, " \n"));
    strcpy(Messaggio.fila_str, strtok(NULL, " \n"));
    strcpy(Messaggio.data_inizio_str, strtok(NULL, " \n"));
    strcpy(Messaggio.data_fine_str, strtok(NULL, "\n"));

    Messaggio.ombrellone = atoi(Messaggio.ombrellone_str);
    Messaggio.fila = atoi(Messaggio.fila_str);
    Messaggio.data_inizio = uniscidata(Messaggio.data_inizio_str);
    Messaggio.data_fine = uniscidata(Messaggio.data_fine_str);

    printf("la parola scritta è: %s\n", Messaggio.parola);
    printf("Il numero dell'ombrellone è: %d\n", Messaggio.ombrellone);
    printf("La fila dell'ombrellone è: %d\n", Messaggio.fila);
    printf("La data di inizio è: %d\n", Messaggio.data_inizio);
    printf("la data di fine è: %d\n", Messaggio.data_fine);
}