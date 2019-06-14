#include "server.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <signal.h>
#include <time.h>
#include <pthread.h>
#include "thpool.h"
#include <malloc.h>
#include <semaphore.h>

sem_t mutex;
void aggiornaFile(void *);
void connection_handler(void *);
int goo = 1;
int masterSocket; // descrittore del master socket
int c12;
int client_sock; // client socket descriptor
risposta Risposta;
messaggio Messaggio;
char buf[DIM]; //array di stringhe che serve come buffer di transito dei dati dai due socket
FILE *f_ombrelloni, *f_prenotazioni, *f_aggiornamenti;

void sighand(int sig)
{
    printf("\n");
    if (sig == SIGINT)
    {
        printf(RED "hai premuto CTRL-C ... chiusura del Master Socket.\n" CRESET);
        close(masterSocket);
    }
    if (sig == SIGTERM)
    {
        printf(RED "SIGTERM RICEVUTO...\n" CRESET);
    }
    exit(0);
}

int main(int argc, char *argv[])
{
    pid_t pid;
    pid = fork();
    if (pid < 0)
    {
        printf(RED "Errore nella fork" CRESET);
        exit(EXIT_FAILURE);
    }
    if (pid > 0)
    {
        //Addio al padre
        exit(EXIT_SUCCESS);
    }
    sem_init(&mutex, 0, 1);
    time_t data;
    int giorno, mese, anno;
    time(&data);
    struct tm *local = localtime(&data);
    giorno = local->tm_mday;
    mese = local->tm_mon + 1;
    anno = local->tm_year + 1900;
    char dataodierna[DIM];
    sprintf(dataodierna, "%02d/%02d/%d", giorno, mese, anno);

    crealista(&Risposta.lista);
    memset(&Risposta, 0, sizeof(Risposta));

    printf("\nPid figlio server: %d\nPid del padre(Terminale): %d\n", (int)getpid(), (int)getppid());

    Risposta.data_oggi = uniscidata(dataodierna);
    printf("%d\n", Risposta.data_oggi);

    leggoFile(&Risposta, f_ombrelloni, f_prenotazioni, f_aggiornamenti);

    struct sockaddr_in sa, client; //struttura per l'indirizzo del server
    memset(&sa, 0, sizeof(sa));    //inizializza tutti i dati della struttura

    sa.sin_family = AF_INET;                     //famiglia indirizzi
    sa.sin_addr.s_addr = inet_addr("127.0.0.1"); //ip del server  inet_addr->converte numero in notazione puntata in numero a 32 bit
    sa.sin_port = htons(PROTOPORT);              //porta del server  htons->converte dall'ordine di 16-bit dell'host all'ordine del network

    signal(SIGINT, sighand);
    signal(SIGCHLD, sighand);

    // creazione del master socket
    masterSocket = socket(AF_INET, SOCK_STREAM, 0); //af_inet=ipv4 stream->socket tcp  0->protocollo di default
    if (masterSocket < 0)
    {
        printf(RED "Fallimento nella creazione della Socket.\n" CRESET);
        close(masterSocket);
        exit(0);
    }
    else
        printf(GREEN "Socket creata con successo.\n" CRESET);

    if (bind(masterSocket, (struct sockaddr *)&sa, sizeof(sa)) < 0) //la bind assegan un indirizzo locale ad una socket, e controlla l'esito
    {
        printf(RED "bind() fallita.\n" CRESET);
        close(masterSocket);
        exit(0);
    }
    else
    {
        printf(GREEN "La Bind ha avuto successo!!!!\n" CRESET);
    }

    if (listen(masterSocket, QLEN) < 0) //QLEN->la massima lunghezza della code delle connessioni entranti
    {                                   //listen restituisce un valore negativo se fallisce, altrimenti 0
        printf(RED "listen() fallita.\n" CRESET);
        close(masterSocket);
        exit(0);
    }

    else
    {
        printf(RED "In attesa di una connessione da un client....\n" CRESET);
    }

    c12 = sizeof(struct sockaddr_in);
    pthread_t thread_id;
    threadpool thpool = thpool_init(QLEN);
    thpool_add_work(thpool, aggiornaFile, ((void *)&Risposta));

    while ((client_sock = accept(masterSocket, (struct sockaddr *)&client, (socklen_t *)&c12)))
    {
        puts(GREEN "Connessione accettata" CRESET);
        while (thpool_num_threads_working(thpool) >= QLEN)
        {
        }

        thpool_add_work(thpool, connection_handler, (void *)&client_sock);
        puts(GREEN "Gestione assegnata ad un thread" CRESET);
    }

    if (client_sock < 0)
    {
        puts(RED "accept fallita" CRESET);
        return 1;
    }

    return 0;
}

void connection_handler(void *socket_desc)
{
    int sock = *(int *)socket_desc;
    char msg[DIM] = {0};
    srand(time(0));
    int id = 1 + rand() % 1000;
    Risposta.IDclient = id;
    int ombrellone_attuale[10] = {0};
    char mid[DIM] = "Il tuo id è ";
    char conv[DIM];
    int go = 1;
    sprintf(conv, "%d", id);
    strcat(mid, conv);

    printf(GREEN "Client %d conesso\n" CRESET, id);

    if (write(sock, mid, sizeof(mid)) != sizeof(mid)) //controlla se scrive il messaggio in tutta la sua lunghezza
    {
        printf(RED "Errore nella ricezione della lunghezza del messaggio.\n" CRESET);
        close(sock);
        printf(RED "Socket chiusa.\n" CRESET);
    }
    while (go)
    {

        if (read(sock, buf, sizeof(buf)) != sizeof(buf)) //legge quello che c'è scritto sul socket figlio, e lo scrive in buf
        {
            printf(RED "Errore nella lunghezza del messaggio presente sul Socket client.\n" CRESET);
            for (int i = 0; i < 10; i++)
            {
                if (Risposta.Ombrellone[ombrellone_attuale[i]].disponibile == 4)
                {
                    Risposta.Ombrellone[ombrellone_attuale[i]].disponibile = 0;
                    Risposta.ombrelloni_liberi++;
                }
                if (Risposta.ombrelloni_Toccupati[ombrellone_attuale[i]] == 1)
                {
                    Risposta.ombrelloni_Toccupati[ombrellone_attuale[i]] = 0;
                }
            }

            printf(RED "Client %d disconnesso\n" CRESET, id);
            go = 0;
            close(sock);
        }
        else

        {
            printf("Il client ha detto: %s", buf); //stampa a schermo quello che ha letto dal client
            Risposta.IDclient = id;
            sem_wait(&mutex);
            if ((f_aggiornamenti = fopen("aggiornamenti.txt", "a")) == NULL)
            {
                printf(RED "Errore nell'apertura del file Aggiornamenti.\n" CRESET);
                exit(-1);
            }
            else
                printf(GREEN "File Aggiornamenti aperto correttamente.\n" CRESET);
            fprintf(f_aggiornamenti, "%d-%s", id, buf);
            fclose(f_aggiornamenti);
            sem_post(&mutex);

            Messaggio = dividiFrase(buf);

            strncpy(msg, elaboraRisposta(&Risposta, Messaggio), sizeof(char) * DIM);

            int j = 0;

            if ((strncmp("AVAILABLE", msg, 9) == 0) && (strncmp("BOOK", Messaggio.parola, 4) == 0))
            {
                while (ombrellone_attuale[j] != 0)
                {
                    j++;
                }
                ombrellone_attuale[j] = Messaggio.ID;
            }
            if (write(sock, msg, sizeof(msg)) != sizeof(msg)) //controlla se scrive il messaggio in tutta la sua lunghezza
            {
                printf(RED "Errore nella ricezione della lunghezza del messaggio.\n" CRESET);
                close(sock);
                printf(RED "Socket chiusa.\n" CRESET);
            }
        }
        if (strncmp("USCITA", msg, 6) == 0)
        {
            for (int i = 0; i < 10; i++)
            {
                if (Risposta.Ombrellone[ombrellone_attuale[i]].disponibile == 4)
                {
                    Risposta.Ombrellone[ombrellone_attuale[i]].disponibile = 0;
                    Risposta.ombrelloni_liberi++;
                }
                if (Risposta.ombrelloni_Toccupati[ombrellone_attuale[i]] == 1)
                {
                    Risposta.ombrelloni_Toccupati[ombrellone_attuale[i]] = 0;
                }
            }
            go = 0;
            printf(RED "Client %d disconnesso\n" CRESET, id);
            close(sock);
        }
    }
}

void aggiornaFile(void *Risposta)
{

    while (1)
    {
        sleep(25);
        risposta Ris = *(risposta *)Risposta;
        int ok = 0;
        int i;
        if ((f_ombrelloni = fopen("ombrelloni.txt", "w")) == NULL)
        {
            printf(RED "Errore apertura file ombrelloni.\n" CRESET);
        }

        for (i = 0; i <= 100; i++)
        {
            if (i == 0)
            {
                fprintf(f_ombrelloni, "%d \n", Ris.data_oggi);
            }
            else
            {
                if (Ris.Ombrellone[i].disponibile == 4)
                {
                    (fprintf(f_ombrelloni, "%d %d %d %d %d \n",
                             Ris.Ombrellone[i].ID,
                             Ris.Ombrellone[i].fila,
                             Ris.Ombrellone[i].numero,
                             0,
                             Ris.Ombrellone[i].IDclient));
                }
                else
                {
                    (fprintf(f_ombrelloni, "%d %d %d %d %d \n",
                             Ris.Ombrellone[i].ID,
                             Ris.Ombrellone[i].fila,
                             Ris.Ombrellone[i].numero,
                             Ris.Ombrellone[i].disponibile,
                             Ris.Ombrellone[i].IDclient));
                }
            }
        }
        if ((f_prenotazioni = fopen("prenotazioni.txt", "w")) == NULL)
        {
            printf(RED "Errore apertura file prenotazioni.\n" CRESET);
        }

        stampaListaSuFile(&Ris.lista, f_prenotazioni);

        fclose(f_prenotazioni);
        fclose(f_ombrelloni);

        sem_wait(&mutex);
        if ((f_aggiornamenti = fopen("aggiornamenti.txt", "w")) == NULL)
        {
            printf(RED "Errore apertura file aggiornamenti.\n" CRESET);
        }
        fclose(f_aggiornamenti);
        sem_post(&mutex);
    }
}
