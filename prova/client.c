#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>

#define BUFFERSIZE 512
#define PROTOPORT 5193

int main()
{

    struct sockaddr_in sa; //struttura della socket
    int sd;                //valore della funzione socket
    int ret;
    char msg[256] = {0}; //stringa in cui si scrive il messaggio da inviare

    // creazione del socket
    sd = socket(AF_INET, SOCK_STREAM, 0); //af_inet=ipv4 stream->socket tcp  0->protocollo di default
    if (sd < 0)
    {
        printf("Fallimento nella creazione della Socket.\n");
        closesocket(sd);
    }

    // inizializzazione dell'indirizzo del server
    sa.sin_family = AF_INET;                     //famiglia indirizzi
    sa.sin_port = htons(5193);                   //porta del server  htons->converte da formato del pc locale a quello della rete
    sa.sin_addr.s_addr = inet_addr("127.0.0.1"); //ip del server  inet_addr->converte numero in notazione puntata in numero a 32 bit

    // richiesta di connessione
    if (connect(sd, (struct sockaddr *)&sa, sizeof(sa)) < 0) //connette alla socket, restituisce 0 se ha successo, altrimenti -1
    {
        printf("Connessione Fallita.\n");
        closesocket(sd);
        printf("Socket chiusa.\n");
    }

    // scrive un messaggio sulla stringa e lo invia al server

    printf("Scrivi il messaggio: ");
    scanf("%s", msg);

    if (write(sd, msg, sizeof(msg)) != sizeof(msg)) //controlla se scrive il messaggio in tutta la sua lunghezza
    {
        printf("Errore nella ricezione della lunghezza del messaggio");
        closesocket(sd);
        printf("Socket chiusa.\n");
    }

    // ricezione dati dal server
    int bytesRicevuti;
    int totBytesRicevuti = 0;
    char buf[BUFFERSIZE]; //stringa di dati ricevuti dal server
    printf("Il server risponde: ");

    while (totBytesRicevuti < sizeof(msg))
    {
        if ((bytesRicevuti = recv(sd, buf, BUFFERSIZE - 1, 0)) <= 0) //restituisce il humero di byte ricevuti, altrimenti riceve <=0
        {
            printf("Ricezione fallita.\n");
            closesocket(sd);
        }
        totBytesRicevuti += bytesRicevuti; //tiene la grandezza dei byte totali
        buf[bytesRicevuti] = '\0';         //aggiuge il carattere di chiusura della stringa
        printf("%s", buf);                 //stampa la stringa ricevuta
    }

    // chiusura della socket
    closesocket(sd);
    printf("Socket chiusa per termine del messaggio.\n");

    return 0;
}