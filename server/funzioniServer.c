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