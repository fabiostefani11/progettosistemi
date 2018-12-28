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

#define PROTOPORT 5193 //numero della porta di default
#define QLEN 6         //grandezza della coda

int go = 1;
int masterSocket; // descrittore del master socket

/*void sighand(int sig)
{
    if (sig == SIGINT)
    {
        printf("you typed CTRL-C ... closing master socket descriptor.\n");
        close(msd);
    }
    else if (sig == SIGCHLD)
    {
        printf("received signal SIGCHLD.\n");
    }
}*/

int main(int argc, char *argv[])
{

    int csd; // client socket descriptor

    int port;

    if (argc > 1)
    {
        port = atoi(argv[1]); //se come argomento si dà l'indizirizzo di una porta, atoi la converte in binario
    }
    else
        port = PROTOPORT;
    if (port < 0)
    {
        printf("Errore nel numero della porta %s. \n", argv[1]);
    }

    struct sockaddr_in sa;      //struttura per l'indirizzo del server
    memset(&sa, 0, sizeof(sa)); //inizializza tutti i dati della struttura

    sa.sin_family = AF_INET;                     //famiglia indirizzi
    sa.sin_addr.s_addr = inet_addr("127.0.0.1"); //ip del server  inet_addr->converte numero in notazione puntata in numero a 32 bit
    sa.sin_port = htons(port);                   //porta del server  htons->converte dall'ordine di 16-bit dell'host all'ordine del network

    signal(SIGINT, sighand);
    signal(SIGCHLD, sighand);

    // creazione del master socket
    masterSocket = socket(AF_INET, SOCK_STREAM, 0); //af_inet=ipv4 stream->socket tcp  0->protocollo di default
    if (masterSocket < 0)
    {
        printf("Fallimento nella creazione della Socket.\n");
        closesocket(masterSocket);
    }

    if (bind(masterSocket, (struct sockaddr *)&sa, sizeof(sa)) < 0)
    {
        printf("bind() fallita.\n");
        closesocket(masterSocket);
    }

    if (listen(masterSocket, QLEN) < 0) //QLEN->la massima lunghezza della code delle connessioni entranti
    {                                   //listen restituisce un valore negativo se fallisce, altrimenti 0
        printf("listen() fallita.\n");
        closesocket(masterSocket);
    }
    /////////////////////////////////DA QUI IN GIU' SERVE SCRIVERE IL CICLO PER LA CONVERSAZIONE///////////////////////////////////////////////
    ///////////////PRENDERE SPUNTO DA CODICI MESSI DAL PROFESSORE SUL SITO//////////////////////////////////////