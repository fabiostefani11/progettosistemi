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

char *confrontaParola(char parola[])
{
    char *msg = malloc(sizeof(char) * DIM);

    if (strncmp("BOOK", parola, 4) == 0)
    {
        strncpy(msg, "OK\n", sizeof(char) * DIM);
    }
    else if (strncmp("AVAILABLE", parola, 9) == 0)
    {
        strncpy(msg, "available\n", sizeof(char) * DIM);
    }
    else if (strncmp("CANCEL", parola, 6) == 0)
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