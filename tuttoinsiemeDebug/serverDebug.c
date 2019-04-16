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
#define BUFFERSIZE 256

#define DIM 256
#define dim 10

int go = 1;
int masterSocket; // descrittore del master socket

typedef struct
{
    char parola[DIM];
    int ID;
    int ombrellone;
    int fila;
    int data_inizio;
    int data_fine;
    int nparole;

} messaggio;

typedef struct
{
    int ID;
    int numero;
    int fila;
    int disponibile; //0=disponibile 1=occupato adesso 2=occupato in futuro 3=occupato sia adesso che in futuro 4=temporaneamente occupato

} ombrellone;

typedef struct
{
    ombrellone Ombrellone[DIM];
    char msg[DIM];
    int ombrelloni_liberi;
} risposta;

int uniscidata(char data[]);
messaggio dividiFrase(char msg[]);
risposta elaboraRisposta(risposta Risposta, messaggio Messaggio);

//questa funzione non la metto nel .h e nel .c perchè dà errore sulla variabile mastersocket
void sighand(int sig)
{
    if (sig == SIGINT)
    {
        printf("hai premuto CTRL-C ... chiusura del Master Socket.\n");
        close(masterSocket);
    }
    else if (sig == SIGCHLD)
    {
        printf("ricevuto signale di SIGCHLD.\n");
    }
}

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
        if (msg[k] != ' ')
        {
            frase[j][i] = msg[k];
            i++;
        }
        else if (msg[k] == ' ' && (msg[k + 1] != '\n' && msg[k + 1] != ' ')) //elimino il problema degli spazi se ce ne sono in più
        {
            frase[j][i] = '\0';
            j++;
            i = 0;
        }
        k++;
    }

    Messaggio.nparole = j + 1;
    strcpy(Messaggio.parola, frase[0]);
    if (strlen(frase[1]) <= 2)
    {
        Messaggio.fila = atoi(frase[1]);
    }
    else if (strlen(frase[1]) > 2)
    {
        if (frase[1][2] != '/' || frase[1][5] != '/') //controllo se la data è nel formato corretto
        {
            strcpy(Messaggio.parola, "ERRORE_DATA");
            return Messaggio;
        }
        Messaggio.data_inizio = uniscidata(frase[1]);
    }
    if (strlen(frase[2]) <= 2)
    {
        Messaggio.ombrellone = atoi(frase[2]);
        Messaggio.ID = (((Messaggio.fila * 10) + Messaggio.ombrellone) - 10);
    }
    else if (strlen(frase[2]) > 2)
    {
        if (frase[2][2] != '/' || frase[2][5] != '/') //controllo se la data è nel formato corretto
        {
            strcpy(Messaggio.parola, "ERRORE_DATA");
            return Messaggio;
        }
        Messaggio.data_fine = uniscidata(frase[2]);
    }
    if (Messaggio.nparole == 5)
    {
        if (frase[3][2] != '/' || frase[3][5] != '/' || frase[4][2] != '/' || frase[4][5] != '/') //controllo se la data è nel formato corretto
        {
            strcpy(Messaggio.parola, "ERRORE_DATA");
            return Messaggio;
        }
        Messaggio.data_inizio = uniscidata(frase[3]);
        Messaggio.data_fine = uniscidata(frase[4]);
    }
    if (Messaggio.nparole == 4)
    {
        if (frase[3][2] != '/' || frase[3][5] != '/') //controllo se la data è nel formato corretto
        {
            strcpy(Messaggio.parola, "ERRORE_DATA");
            return Messaggio;
        }
        Messaggio.data_fine = uniscidata(frase[3]);
    }
    return Messaggio;
}

risposta elaboraRisposta(risposta Risposta, messaggio Messaggio)
{
    risposta Risposta_output;
    char *msg = malloc(sizeof(char) * DIM);
    int k;
    int i;

    if (strncmp("ERRORE_DATA", Messaggio.parola, 11) == 0) //se la data non è nel formato corretto ritorna un errore
    {
        strncpy(msg, "Data inserita in un formato non corretto.\n", sizeof(char) * DIM);
        strncpy(Risposta_output.msg, msg, sizeof(char) * DIM);
        return Risposta;
    }

    if ((strncmp("BOOK", Messaggio.parola, 4) == 0) && (Messaggio.nparole == 1)) //scrive solo BOOK
    {
        if (Risposta.ombrelloni_liberi == 0)
        {
            strncpy(msg, "NAVAILABLE\n", sizeof(char) * DIM); //risponde nok se non ci sono ombrelloni liberi
        }
        else
            strncpy(msg, "OK\n", sizeof(char) * DIM); //risponde ok se va tutto bene
    }
    else if ((strncmp("BOOK", Messaggio.parola, 4) == 0) && (Messaggio.nparole == 3)) //scrive BOOK e fila e numero ombrellone
    {
        if (Risposta.Ombrellone[Messaggio.ID].disponibile == 0) //se l'ombrellone richiesto è libero, scrivo temp. occupato e risponde available
        {
            Risposta.Ombrellone[Messaggio.ID].disponibile = 4;
            Risposta.ombrelloni_liberi--;
            strncpy(msg, "AVAILABLE\n", sizeof(char) * DIM);
        }
        else
            strncpy(msg, "NAVAILABLE\n", sizeof(char) * DIM); //ombrellone occupato
    }
    else if ((strncmp("BOOK", Messaggio.parola, 4) == 0) && (Messaggio.nparole == 4)) //conferma la prenotazione, manca pezzo di codice
    {
        Risposta.Ombrellone[Messaggio.ID].disponibile = 1;
        Risposta.ombrelloni_liberi--;
        strncpy(msg, "Manca Codice di conferma prenotazione\n", sizeof(char) * DIM);
    }
    else if ((strncmp("BOOK", Messaggio.parola, 4) == 0) && (Messaggio.nparole == 5)) //prenotazione per il futuro, scrive BOOK fila numero e le 2 date
    {
        strncpy(msg, "Manca Codice di prenotazione futura\n", sizeof(char) * DIM);
    }
    else if (strncmp("AVAILABLE", Messaggio.parola, 9) == 0 && (Messaggio.nparole == 1)) //scrive available per sapere il numero di ombrelloni liberi
    {
        if (Risposta.ombrelloni_liberi == 0) //tutti occupati
        {
            strncpy(msg, "NAVAILABLE\n", sizeof(char) * DIM);
        }
        else
            sprintf(msg, "AVAILABLE %d\n", Risposta.ombrelloni_liberi); //stampa available e il numero di ombrelloni liberi
    }
    else if (strncmp("AVAILABLE", Messaggio.parola, 9) == 0 && (Messaggio.nparole == 2)) //chiede il numero di ombrelloni liberi in una fila
    {
        if (Messaggio.fila > 10)
        {
            strncpy(msg, "Fila Ombrellone inesistente, scrivere una fila da 1 a 10\n", sizeof(char) * DIM);
        }
        else
        {
            int liberi_fila[10] = {0};
            int z = 0;
            int k;
            char *voce = malloc(sizeof(char) * DIM);
            for (k = (Messaggio.fila * 10) - 9; k <= Messaggio.fila * 10; k++)
            {
                if (Risposta.Ombrellone[k].disponibile == 0) //conta gli ombrelloni liberi in una fila e li mette in un array
                {
                    liberi_fila[z] = Risposta.Ombrellone[k].numero;
                    z++;
                }
            }
            if (z == 0) //nessuno libero
            {
                strncpy(msg, "NAVAILABLE\n", sizeof(char) * DIM);
            }
            else
            {
                k = 0;
                while (liberi_fila[k] != 0)
                {
                    sprintf(voce, "%d ", liberi_fila[k]); //scrive gli ombrelloni liberi scritti nell'array, in una stringa
                    strcat(msg, voce);
                    k++;
                }
                strcat(msg, "\n");
            }
        }
    }
    else if ((strncmp("CANCEL", Messaggio.parola, 6) == 0) && (Messaggio.nparole == 3))
    {
        Risposta.Ombrellone[Messaggio.ID].disponibile = 0;
        Risposta.ombrelloni_liberi++;
        strncpy(msg, "CANCEL OK\n", sizeof(char) * DIM);
    }
    /*else if (Messaggio.ombrellone > 10)           //controllo se sono corretti i dati immessi
    {
        strncpy(msg, "Numero Ombrellone inesistente, scrivere un numero da 1 a 10\n", sizeof(char) * DIM);
    }
    else if (Messaggio.fila > 10)
    {
        strncpy(msg, "Fila Ombrellone inesistente, scrivere una fila da 1 a 10\n", sizeof(char) * DIM);
    }*/
    else if (strncmp("EXIT", Messaggio.parola, 4) == 0)
    {
        strncpy(msg, "exit", sizeof(char) * DIM);
    }
    else
    {
        strncpy(msg, "Messaggio non valido, scrivere di nuovo\n", sizeof(char) * DIM);
    }
    ///////assegno tutti i valori alla varialbile di ritorno della funzione
    strncpy(Risposta_output.msg, msg, sizeof(char) * DIM);
    Risposta_output.ombrelloni_liberi = Risposta.ombrelloni_liberi;
    for (i = 1; i <= 100; i++)
    {
        Risposta_output.Ombrellone[i] = Risposta.Ombrellone[i];
    }
    return Risposta_output;
}

int main(int argc, char *argv[])
{

    int csd; // client socket descriptor
    char msg[256] = {0};
    int port;
    int status; //il parametro status il processo che termina può comunicare al padre informazioni sul suo stato di terminazione (ad es. l’esito della sua esecuzione).
    messaggio Messaggio;
    risposta Risposta;
    FILE *f_ombrelloni, *f_prenotazioni;
    int i = 1;

    memset(&Risposta, 0, sizeof(Risposta));

    if ((f_ombrelloni = fopen("ombrelloni.txt", "r")) == NULL)
    {
        printf("Errore nell'apertura del file.\n");
        exit(-1);
    }
    else
        printf("File ombrelloni aperto correttamente.\n");

    if ((f_prenotazioni = fopen("prenotazioni.txt", "rw")) == NULL)
    {
        printf("Errore nell'apertura del file.\n");
        exit(-1);
    }
    else
        printf("File prenotazioni aperto correttamente.\n");

    while (!feof(f_ombrelloni))
    {
        if (fscanf(f_ombrelloni, "%d %d %d %d", &Risposta.Ombrellone[i].ID, &Risposta.Ombrellone[i].fila, &Risposta.Ombrellone[i].numero, &Risposta.Ombrellone[i].disponibile) == 4)
        {
            if (Risposta.Ombrellone[i].disponibile == 0)
            {
                Risposta.ombrelloni_liberi++;
            }
            i++;
        }
    }
    fclose(f_ombrelloni);
    fclose(f_prenotazioni);

    char buf[BUFFERSIZE]; //array di stringhe che serve come buffer di transito dei dati dai due socket

    pid_t pid;

    if (argc > 1) //da togliere
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
        close(masterSocket);
    }
    else
        printf("Socket creata con successo.\n");

    if (bind(masterSocket, (struct sockaddr *)&sa, sizeof(sa)) < 0) //la bind assegan un indirizzo locale ad una socket, e controlla l'esito
    {
        printf("bind() fallita.\n");
        close(masterSocket);
    }
    else
    {
        printf("La Bind ha avuto successo!!!!\n");
    }

    if (listen(masterSocket, QLEN) < 0) //QLEN->la massima lunghezza della code delle connessioni entranti
    {                                   //listen restituisce un valore negativo se fallisce, altrimenti 0
        printf("listen() fallita.\n");
        close(masterSocket);
    }

    else
        printf("In attesa di una connessione da un client....\n");

    /////////////////////////////////DA QUI IN GIU' IL CICLO PER LA CONVERSAZIONE///////////////////////////////////////////////
    while (go)
    {

        // II argomento puntatore indirizzo del client remoto

        if ((csd = accept(masterSocket, NULL, 0)) < 0) //accetta la richiesta di conenssione del socket, e la funzione accept
        {                                              //restituisce il numero del socket se ha successo, altrimenti restituisce -1
            go = 0;
            printf("Accept fallita.\n");
        }
        else
        {
            printf("Connessione riuscita!!!!!\n");
            pid = fork();

            if (pid == 0) //se l'id del processo è 0, significa che il processo è un processo figlio
            {

                close(masterSocket); // chiude il processo padre per continuare sul processo figlio
                while (1)
                {
                    if (read(csd, buf, sizeof(buf)) != sizeof(buf)) //legge quello che c'è scritto sul socket figlio, e lo scrive in buf
                    {
                        printf("Errore nella lunghezza del messaggio presente sul Socket client.\n");
                        close(csd);
                        break;
                    }
                    else
                    {
                        printf("Il client ha detto: %s", buf); //stampa a schermo quello che ha letto dal client

                        //divide la frase in una parola e 4 interi//
                        Messaggio = dividiFrase(buf);
                        Risposta = elaboraRisposta(Risposta, Messaggio);
                        //confronta la parola con le varie possibilità e scrive la risposta nella socket

                        if (write(csd, Risposta.msg, sizeof(Risposta.msg)) != sizeof(Risposta.msg)) //controlla se scrive il messaggio in tutta la sua lunghezza
                        {
                            printf("Errore nella ricezione della lunghezza del messaggio.\n");
                            close(csd);
                            printf("Socket chiusa.\n");
                        }
                        else
                            printf("Invio riuscito.\n");

                        if (strncmp("exit", Risposta.msg, 4) == 0)
                        {
                            if ((f_ombrelloni = fopen("ombrelloni.txt", "w")) == NULL)
                            {
                                printf("Errore nell'apertura del file.\n");
                                exit(-1);
                            }
                            for (i = 0; i < 100; i++)
                            {
                                (fprintf(f_ombrelloni, "%d %d %d %d \n",
                                         Risposta.Ombrellone[i].ID,
                                         Risposta.Ombrellone[i].numero,
                                         Risposta.Ombrellone[i].fila,
                                         Risposta.Ombrellone[i].disponibile));
                            }
                            fclose(f_ombrelloni);
                            printf("Server esce...\n");
                            break;
                        }
                    }
                }
                ///////sul codice del prof qui c'è l'execl, ma non so a cosa serva quindi l'ho tolta
            }
            else //se l'id del processo è maggiore di 0, significa che il processo è padre
            {

                if (wait(&status) < 0) //la funzione wait mette in attesa il processo padre finchè un processo figlio termina o riceve un comando di terminazione
                {
                    printf("Errore nella wait().\n"); //la wait restituisce -1 se fallisce, altrimenti restituisce l'id del processo terminato
                    close(csd);
                    printf("Chiuso il Socket del processo figlio per errore nella wait.\n");
                }
                if (write(csd, &status, sizeof(status)) != sizeof(status)) //scrive sul socket figlio il valore di status
                                                                           //controlla se scrive il messaggio in tutta la sua lunghezza
                {
                    printf("Errore nella ricezione della lunghezza del messaggio scritto dal Socket padre sul Socket figlio\n");
                    close(csd);
                    printf("Socket chiusa.\n");

                    close(masterSocket); //chiude il socket padre e quindi la comunicazione, ricominciando il ciclo
                }
            }
        }

        return 0;
    }
}