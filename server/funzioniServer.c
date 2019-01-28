#include "server.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/wait.h> /* wait */
#include <signal.h>

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
        if (msg[k] == ' ')
        {
            frase[j][i] = '\0';
            k++;
            j++;
            i = 0;
        }
        else
        {
            frase[j][i] = msg[k];
            i++;
            k++;
        }
    }
    Messaggio.nparole = j + 1;
    strcpy(Messaggio.parola, frase[0]);
    if (strlen(frase[1]) <= 2)
    {
        Messaggio.ombrellone = atoi(frase[1]);
    }
    else if (strlen(frase[1]) > 2)
    {
        Messaggio.data_inizio = uniscidata(frase[1]);
    }
    if (strlen(frase[2]) <= 2)
    {
        Messaggio.fila = atoi(frase[2]);
        Messaggio.ID = (((Messaggio.fila * 10) + Messaggio.ombrellone) - 10);
    }
    else if (strlen(frase[2]) > 2)
    {
        Messaggio.data_fine = uniscidata(frase[2]);
    }
    if (Messaggio.nparole > 3)
    {
        Messaggio.data_inizio = uniscidata(frase[3]);
        Messaggio.data_fine = uniscidata(frase[4]);
    }
    return Messaggio;
}

char *confrontaParola(messaggio Messaggio)
{
    char *msg = malloc(sizeof(char) * DIM);

    if ((strncmp("BOOK", Messaggio.parola, 4) == 0) && (Messaggio.nparole == 1))
    {
        strncpy(msg, "OK\n", sizeof(char) * DIM);
    }
    else if ((strncmp("BOOK", Messaggio.parola, 4) == 0) && (Messaggio.nparole == 3))
    {
        strncpy(msg, "OK\n", sizeof(char) * DIM);
    }
    else if ((strncmp("BOOK", Messaggio.parola, 4) == 0) && (Messaggio.nparole == 5))
    {
        strncpy(msg, "OK\n", sizeof(char) * DIM);
    }
    else if (strncmp("AVAILABLE", Messaggio.parola, 9) == 0)
    {
        strncpy(msg, "available\n", sizeof(char) * DIM);
    }
    else if (strncmp("CANCEL", Messaggio.parola, 6) == 0)
    {
        strncpy(msg, "CANCEL OK\n", sizeof(char) * DIM);
    }
    else
    {
        strncpy(msg, "Messaggio non valido, scrivere di nuovo\n", sizeof(char) * DIM);
        //printf("Scrivi la risposta: ");
        //fgets(msg, (sizeof(char) * DIM), stdin);
    }

    return msg;
}