#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>

#define BUFFERSIZE 512
#define PROTOPORT 8888

int main(int argc, char *argv[])
{

    struct sockaddr_in sa;      //struttura della socket
    memset(&sa, 0, sizeof(sa)); //inizializza tutti i campi della struttura
    int mySocket;               //valore della funzione socket
    int ret;
    char msg[256] = {0}; //stringa in cui si scrive il messaggio da inviare

    // creazione del socket
    mySocket = socket(AF_INET, SOCK_STREAM, 0); //af_inet=ipv4 stream->socket tcp  0->protocollo di default
    if (mySocket < 0)
    {
        printf("Fallimento nella creazione della Socket.\n");
        close(mySocket);
    }
    else
        printf("Socket creata.\n");

    // inizializzazione dell'indirizzo del server
    sa.sin_family = AF_INET;                     //famiglia indirizzi
    sa.sin_port = htons(8888);                   //porta del server  htons->converte da formato del pc locale a quello della rete
    sa.sin_addr.s_addr = inet_addr("127.0.0.1"); //ip del server  inet_addr->converte numero in notazione puntata in numero a 32 bit

    // richiesta di connessione
    if (connect(mySocket, (struct sockaddr *)&sa, sizeof(sa)) < 0) //connette il client alla socket, restituisce 0 se ha successo, altrimenti -1
    {                                                              //il secondo campo è l'indirizzo del client
        printf("Connessione Fallita.\n");
        close(mySocket);
        printf("Socket chiusa.\n");
    }

    // scrive un messaggio sulla stringa e lo invia al server

    else
    {
        printf("Connessione riuscita.\n");
        int bytesRicevuti;
        int totBytesRicevuti = 0;
        char buf[BUFFERSIZE]; //stringa di dati ricevuti dal server
        printf("Il server risponde: ");

        while (totBytesRicevuti < sizeof(msg))
        {
            if ((bytesRicevuti = recv(mySocket, buf, BUFFERSIZE - 1, 0)) <= 0) //restituisce il humero di byte ricevuti, altrimenti riceve <=0
            {
                printf("Ricezione fallita.\n");
                close(mySocket);
            }
            totBytesRicevuti += bytesRicevuti; //tiene la grandezza dei byte totali
            buf[bytesRicevuti] = '\0';         //aggiuge il carattere di chiusura della stringa
            printf("%s\n", buf);               //stampa la stringa ricevuta
        }

        while (1)
        {
            printf("Scrivi il messaggio: ");
            fgets(msg, sizeof(msg), stdin);

            if (write(mySocket, msg, sizeof(msg)) != sizeof(msg)) //controlla se scrive il messaggio in tutta la sua lunghezza
            {
                printf("Errore nella ricezione della lunghezza del messaggio\n");
                close(mySocket);
                printf("Socket chiusa.\n");
            }
            /*else
                printf("Invio riuscito.\n");*/
            if (strncmp("EXIT", msg, 4) == 0)
            {
                close(mySocket);
                printf("Disconnessione dal server...\n");
                exit(1);
            }

            ///////////////DA QUI IN GIU' E' PER LA CONVERSAZIONE//////////////////////

            // ricezione dati dal server
            int bytesRicevuti;
            int totBytesRicevuti = 0;
            char buf[BUFFERSIZE]; //stringa di dati ricevuti dal server
            printf("Il server risponde: ");

            while (totBytesRicevuti < sizeof(msg))
            {
                if ((bytesRicevuti = recv(mySocket, buf, BUFFERSIZE - 1, 0)) <= 0) //restituisce il humero di byte ricevuti, altrimenti riceve <=0
                {
                    printf("Ricezione fallita.\n");
                    close(mySocket);
                }
                totBytesRicevuti += bytesRicevuti; //tiene la grandezza dei byte totali
                buf[bytesRicevuti] = '\0';         //aggiuge il carattere di chiusura della stringa
                printf("%s", buf);                 //stampa la stringa ricevuta
            }
        }
    }
    return 0;
}