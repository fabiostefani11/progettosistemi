#include <stdio.h>
#include <string.h> //strlen
#include <stdlib.h> //strlen
#include <sys/socket.h>
#include <arpa/inet.h> //inet_addr
#include <unistd.h>    //write
#include <pthread.h>
#include <signal.h>
#include <sys/stat.h>
#include <syslog.h>
#include <semaphore.h>
#include "server.h"

#define K 101

sem_t SlotLibero, SlotOccupato, mutex;
void *connection_handler(void *);
void *connection_master(void *);

void sighand(int);
int i;
risposta Risposta;
int wp = 0;
int rp = 0;
char buffer[DIM];
int go = 1;
FILE *f_ombrelloni, *f_prenotazioni;

int main(int argc, char *argv[])
{
    pid_t myself;
    risposta Risposta;
    crealista(&Risposta.lista);
    memset(&Risposta, 0, sizeof(Risposta));

    sem_init(&SlotLibero, 1, K);
    sem_init(&SlotOccupato, 1, 0);
    sem_init(&mutex, 1, 0);
    pthread_t thread_id, masterThr;
    int socket_desc, client_sock, c;
    struct sockaddr_in server, client;
    struct sigaction sa;
    int f;

    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = sighand;

    myself = fork();

    if (myself != 0)
    {
        exit(0);
    }
    if (setsid() < 0)
        exit(EXIT_FAILURE);

    sigaction(SIGINT, &sa, 0);
    sigaction(SIGTERM, &sa, 0);
    sigaction(SIGHUP, &sa, 0);
    sigaction(SIGQUIT, &sa, 0);

    /* Fork off for the second time*/
    myself = fork();

    /* An error occurred */
    if (myself < 0)
        exit(EXIT_FAILURE);

    /* Success: Let the parent terminate */
    if (myself > 0)
        exit(EXIT_SUCCESS);

    printf("\nson SERVER PID: %d\n Father PID: %d\n", (int)getpid(), (int)getppid());

    if (pthread_create(&masterThr, NULL, connection_master, (void *)&f) != 0)
    {
        perror("Thread master creation failed. Error");
        return 0;
    }
    puts("--->Thread Master created");
    sem_wait(&SlotOccupato);
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_desc == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(8888);

    //Bind
    if (bind(socket_desc, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        //print the error message
        perror("bind failed. Error");
        return 1;
    }
    puts("bind done");

    //Listen
    listen(socket_desc, 3);

    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);

        while ((client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t *)&c)))
    {
        puts("Connection accepted");

        if (pthread_create(&thread_id, NULL, connection_handler, (void *)&client_sock) < 0)
        {
            perror("could not create thread");
            return 1;
        }

        puts("Handler assigned");
        i++;
        printf("\tClient connected: %d\n", i);
    }

    if (client_sock < 0)
    {
        perror("accept failed");
        return 1;
    }

    return 0;
}
void *connection_master(void *beach1)
{
    
    int ID, fila, numero, IDclient, data_inizio, data_fine;
    puts("\tReading Beach Status..");
    if ((f_ombrelloni = fopen("ombrelloni.txt", "r")) == NULL)
    {
        printf("Errore nell'apertura del file ombrelloni.\n");
        exit(-1);
    }
    else
        printf("File ombrelloni aperto correttamente.\n");

    if ((f_prenotazioni = fopen("prenotazioni.txt", "r")) == NULL)
    {
        printf("Errore nell'apertura del file prenotazioni.\n");
        exit(-1);
    }
    else
        printf("File prenotazioni aperto correttamente.\n");

    while (!feof(f_ombrelloni))
    {
        if (fscanf(f_ombrelloni, "%d %d %d %d %d", &Risposta.Ombrellone[i].ID, &Risposta.Ombrellone[i].fila, &Risposta.Ombrellone[i].numero, &Risposta.Ombrellone[i].disponibile, &Risposta.Ombrellone[i].IDclient) == 5)
        {
            if (Risposta.Ombrellone[i].disponibile == 0)
            {
                Risposta.ombrelloni_liberi++;
            }
            i++;
        }
    }

    while (!feof(f_prenotazioni))
    {
        if (fscanf(f_prenotazioni, "%d %d %d %d %d %d", &ID, &fila, &numero, &IDclient, &data_inizio, &data_fine) == 6)
        {
            inserimento(&Risposta.lista, ID, fila, numero, IDclient, data_inizio, data_fine);
        }
    }
    fclose(f_ombrelloni);
    fclose(f_prenotazioni);

    puts("\tRead success");
    //PrintStatus(beach1);
    //journaling da beach_new.txt a beach_status.txt o beach_abb.txt
}

void *connection_handler(void *socket_desc)
{

    char buf[BUFFERSIZE]; //array di stringhe che serve come buffer di transito dei dati dai due socket
    char msg[256] = {0};
    messaggio Messaggio;

    //Get the socket descriptor
    int sock = *(int *)socket_desc;
    char *message, client_message[2000];

    srand(time(0));
    int id = 1 + rand() % 1000;
    Risposta.IDclient = id;
    int ombrellone_attuale = 0;
    char mid[DIM] = "Il tuo id è ";
    char conv[DIM];
    sprintf(conv, "%d", id);
    strcat(mid, conv);
    if (write(sock, mid, sizeof(mid)) != sizeof(mid)) //controlla se scrive il messaggio in tutta la sua lunghezza
    {
        printf("Errore nella ricezione della lunghezza del messaggio.\n");
        close(sock);
        printf("Socket chiusa.\n");
    }


    //if (pid == 0) //se l'id del processo è 0, significa che il processo è un processo figlio
    //{

    // close(sock); // chiude il processo padre per continuare sul processo figlio
    while (go)
    {

        if (read(sock, buf, sizeof(buf)) != sizeof(buf)) //legge quello che c'è scritto sul socket figlio, e lo scrive in buf
        {
            printf("Errore nella lunghezza del messaggio presente sul Socket client.\n");
            close(sock);
            break;
        }
        else

        {
            printf("Il client ha detto: %s", buf); //stampa a schermo quello che ha letto dal client

            //divide la frase in una parola e 4 interi//
            Risposta.IDclient = id;
            Messaggio = dividiFrase(buf);
            if (Messaggio.nparole > 1 && (strncmp("BOOK", Messaggio.parola, 4) == 0))
            {
                ombrellone_attuale = Messaggio.ID;
            }
            strncpy(msg, elaboraRisposta(&Risposta, Messaggio), sizeof(msg));

            //confronta la parola con le varie possibilità e scrive la risposta nella socket

            if (write(sock, Risposta.msg, sizeof(Risposta.msg)) != sizeof(Risposta.msg)) //controlla se scrive il messaggio in tutta la sua lunghezza
            {
                printf("Errore nella ricezione della lunghezza del messaggio.\n");
                close(sock);
                printf("Socket chiusa.\n");
            }
        }
    }
    //}

    if (strncmp("EXIT", msg, 4) == 0)
    {
        if ((f_ombrelloni = fopen("ombrelloni.txt", "w")) == NULL)
        {
            printf("Errore nell'apertura del file ombrelloni.\n");
            exit(-1);
        }
        else
            printf("File ombrelloni aperto correttamente.\n");

        if (Risposta.Ombrellone[ombrellone_attuale].disponibile == 4)
        {
            Risposta.Ombrellone[ombrellone_attuale].disponibile = 0;
        };
        for (int i = 1; i <= 100; i++)
        {
            (fprintf(f_ombrelloni, "%d %d %d %d %d \n",
                     Risposta.Ombrellone[i].ID,
                     Risposta.Ombrellone[i].fila,
                     Risposta.Ombrellone[i].numero,
                     Risposta.Ombrellone[i].disponibile,
                     Risposta.Ombrellone[i].IDclient));
        }
        if ((f_prenotazioni = fopen("prenotazioni.txt", "w")) == NULL)
        {
            printf("Errore nell'apertura del file prenotazioni.\n");
            exit(-1);
        }
        else
            printf("File prenotazioni aperto correttamente.\n");

        stampaListaSuFile(&Risposta.lista, f_prenotazioni);

        fclose(f_ombrelloni);
        fclose(f_prenotazioni);
        go = 0;
        sighand(2);
    }
}

void sighand(int sig)
{
    printf("\n");
    go = 0;
    if (sig == SIGINT)
    {
        printf("SIGINT received ... GAME OVER ! \n");
    }
    if (sig == SIGTERM)
    {
        printf("SIGTERM received ... GAME OVER ! \n");
    }
    if (sig == SIGQUIT)
    {
        printf("SIGQUIT received ... GAME OVER ! \n");
    }
    if (sig == SIGHUP)
    {
        printf("SIGHUP received ... GAME OVER ! \n");
    }

    exit(0);
}