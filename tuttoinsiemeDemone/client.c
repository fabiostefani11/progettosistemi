#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <signal.h>

#define RED "\x1b[31m"
#define CRESET "\x1b[0m"
#define GREEN "\x1b[32m"

#define PROTOPORT 8889
#define DIM 256

int primo = 0; //per argomenti
int go = 1;
int mySocket; //valore della funzione socket

void sighand(int sig)
{
    printf("\n");
    if (sig == SIGALRM)

    {
        printf(RED "Sessione scaduta per inattività.\n" CRESET);
        close(mySocket);
        printf(RED "Disconnessione dal server...\n" CRESET);
        exit(0);
    }
    if (sig == SIGINT)
    {
        printf(RED "hai premuto CTRL-C ... chiusura del Client.\n" CRESET);
        close(mySocket);
        exit(0);
    }
}

int main(int argc, char *argv[])
{

    struct sockaddr_in sa;      //struttura della socket
    memset(&sa, 0, sizeof(sa)); //inizializza tutti i campi della struttura
    char msg[DIM] = {0};        //stringa in cui si scrive il messaggio da inviare
    signal(SIGALRM, sighand);
    signal(SIGINT, sighand);
    // creazione del socket
    mySocket = socket(AF_INET, SOCK_STREAM, 0); //af_inet=ipv4 stream->socket tcp  0->protocollo di default
    if (mySocket < 0)
    {
        printf(RED "Fallimento nella creazione della Socket.\n" CRESET);
        close(mySocket);
    }
    else
        printf(GREEN "Socket creata.\n" CRESET);

    // inizializzazione dell'indirizzo del server
    sa.sin_family = AF_INET;                     //famiglia indirizzi
    sa.sin_port = htons(PROTOPORT);              //porta del server  htons->converte da formato del pc locale a quello della rete
    sa.sin_addr.s_addr = inet_addr("127.0.0.1"); //ip del server  inet_addr->converte numero in notazione puntata in numero a 32 bit

    // richiesta di connessione
    if (connect(mySocket, (struct sockaddr *)&sa, sizeof(sa)) < 0) //connette il client alla socket, restituisce 0 se ha successo, altrimenti -1
    {                                                              //il secondo campo è l'indirizzo del client
        printf(RED "Connessione Fallita.\n" CRESET);
        close(mySocket);
        printf(RED "Socket chiusa.\n" CRESET);
    }
    else
    {
        strncpy(msg, "", sizeof(char) * DIM);
        printf(GREEN "Connessione riuscita con la socket.\n" CRESET);
        printf(RED "In attesa di risposta dal server...\n" CRESET);
        int bytesRicevuti;
        int totBytesRicevuti = 0;
        char buf[DIM]; //stringa di dati ricevuti dal server
        printf("Il server risponde: ");

        while (totBytesRicevuti < sizeof(msg))
        {
            if ((bytesRicevuti = recv(mySocket, buf, DIM - 1, 0)) <= 0) //restituisce il humero di byte ricevuti, altrimenti riceve <=0
            {
                printf(RED "Ricezione fallita.\n" CRESET);
                close(mySocket);
            }
            totBytesRicevuti += bytesRicevuti; //tiene la grandezza dei byte totali
            buf[bytesRicevuti] = '\0';         //aggiuge il carattere di chiusura della stringa
            printf("%s\n", buf);               //stampa la stringa ricevuta
        }

        while (go)
        {
            alarm(60);

            if (argc > 1 && primo == 0)
            {
                strcpy(msg, argv[1]);

                if (argc > 2)
                {
                    int x;
                    for (x = 2; x < argc; x++)
                    {
                        strcat(msg, " ");
                        strcat(msg, argv[x]);
                    }
                }
                strcat(msg, "\n");
                if (write(mySocket, msg, sizeof(msg)) != sizeof(msg)) //controlla se scrive il messaggio in tutta la sua lunghezza
                {
                    printf(RED "Errore nella ricezione della lunghezza del messaggio\n" CRESET);
                    close(mySocket);
                    printf(RED "Socket chiusa.\n" CRESET);
                }
                primo++;
            }
            else
            {
                strncpy(msg, "", sizeof(char) * DIM);
                printf("Scrivi il messaggio: ");
                fgets(msg, sizeof(msg), stdin);
                if (write(mySocket, msg, sizeof(msg)) != sizeof(msg)) //controlla se scrive il messaggio in tutta la sua lunghezza
                {
                    printf(RED "Errore nella ricezione della lunghezza del messaggio\n" CRESET);
                    close(mySocket);
                    printf(RED "Socket chiusa.\n" CRESET);
                }
            }
            if (strncmp("EXIT", msg, 4) == 0)
            {
                close(mySocket);
                printf(RED "Disconnessione dal server...\n" CRESET);
                exit(1);
            }
            // ricezione dati dal server
            int bytesRicevuti;
            int totBytesRicevuti = 0;
            char buf[DIM]; //stringa di dati ricevuti dal server
            printf("Il server risponde: ");

            while (totBytesRicevuti < sizeof(msg))
            {
                if ((bytesRicevuti = recv(mySocket, buf, DIM - 1, 0)) <= 0) //restituisce il humero di byte ricevuti, altrimenti riceve <=0
                {
                    printf(RED "Ricezione fallita.\n" CRESET);
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
