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
} risposta;

int uniscidata(char data[]);
messaggio dividiFrase(char msg[]);
risposta elaboraRisposta(int liberi, messaggio Messaggio, ombrellone Ombrellone[]);

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
        Messaggio.fila = atoi(frase[1]);
    }
    else if (strlen(frase[1]) > 2)
    {
        Messaggio.data_inizio = uniscidata(frase[1]);
    }
    if (strlen(frase[2]) <= 2)
    {
        Messaggio.ombrellone = atoi(frase[2]);
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

risposta elaboraRisposta(int liberi, messaggio Messaggio, ombrellone Ombrellone[])
{
    risposta Risposta;
    char *msg = malloc(sizeof(char) * DIM);
    int k;

    if ((strncmp("BOOK", Messaggio.parola, 4) == 0) && (Messaggio.nparole == 1)) //scrive solo BOOK
    {
        if (liberi == 0)
        {
            strncpy(msg, "NAVAILABLE\n", sizeof(char) * DIM); //risponde nok se non ci sono ombrelloni liberi
        }
        else
            strncpy(msg, "OK\n", sizeof(char) * DIM); //risponde ok se va tutto bene
    }
    else if ((strncmp("BOOK", Messaggio.parola, 4) == 0) && (Messaggio.nparole == 3)) //scrive BOOK e fila e numero ombrellone
    {
        if (Ombrellone[Messaggio.ID].disponibile == 0) //se l'ombrellone richiesto è libero, scrivo temp. occupato e risponde available
        {
            Ombrellone[Messaggio.ID].disponibile = 4;
            strncpy(msg, "AVAILABLE\n", sizeof(char) * DIM);
        }
        else
            strncpy(msg, "NAVAILABLE\n", sizeof(char) * DIM); //ombrellone occupato
    }
    else if ((strncmp("BOOK", Messaggio.parola, 4) == 0) && (Messaggio.nparole == 4)) //conferma la prenotazione, manca pezzo di codice
    {
        Ombrellone[Messaggio.ID].disponibile = 1;
        strncpy(msg, "Manca Codice di conferma prenotazione\n", sizeof(char) * DIM);
    }
    else if ((strncmp("BOOK", Messaggio.parola, 4) == 0) && (Messaggio.nparole == 5)) //prenotazione per il futuro, scrive BOOK fila numero e le 2 date
    {
        strncpy(msg, "Manca Codice di prenotazione futura\n", sizeof(char) * DIM);
    }
    else if (strncmp("AVAILABLE", Messaggio.parola, 9) == 0 && (Messaggio.nparole == 1)) //scrive available per sapere il numero di ombrelloni liberi
    {
        if (liberi == 0) //tutti occupati
        {
            strncpy(msg, "NAVAILABLE\n", sizeof(char) * DIM);
        }
        else
            sprintf(msg, "AVAILABLE %d\n", liberi); //stampa available e il numero di ombrelloni liberi
    }
    else if (strncmp("AVAILABLE", Messaggio.parola, 9) == 0 && (Messaggio.nparole == 2)) //chiede il numero di ombrelloni liberi in una fila
    {
        if (Messaggio.fila > 10)
        {
            strncpy(msg, "Fila Ombrellone inesistente, scrivere una fila da 1 a 10\n", sizeof(char) * DIM);
        }
        else
        {
            int z = 0;
            int liberi_fila[10] = {0};
            int k = 0;
            char *voce = malloc(sizeof(char) * DIM);
            for (int i = 1; i <= 100; i++)
            {
                if (Ombrellone[i].disponibile == 0 && Ombrellone[i].fila == Messaggio.fila) //conta gli ombrelloni liberi in una fila e li mette in un array
                {
                    sprintf(voce, "%d ", Ombrellone[i].numero);
                    strcat(msg, voce);
                    z++;
                }
            }
            if (z == 0) //nessuno libero
            {
                strncpy(msg, "NAVAILABLE\n", sizeof(char) * DIM);
            }
            else
            {
                strcat(msg, "\n");
            }
        }
    }
    else if (strncmp("CANCEL", Messaggio.parola, 6) == 0)
    {
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
    else
    {
        strncpy(msg, "Messaggio non valido, scrivere di nuovo\n", sizeof(char) * DIM);
    }
    printf("Prima di strncpy msg: %s Risposta.msg: %s\n", msg, Risposta.msg);
    strncpy(Risposta.msg, msg, sizeof(char) * DIM);
    printf("Dopo strncpy msg: %s Risposta.msg: %s\n", msg, Risposta.msg);
    //return Risposta.msg;
    return Risposta;
}

int main()
{
    messaggio Messaggio;
    ombrellone Ombrellone[100];
    risposta Risposta;
    char frase[DIM] = {0};
    char msg[DIM] = {0};
    FILE *f_ombrelloni;
    int i = 1;
    int ombrelloni_liberi = 0;

    if ((f_ombrelloni = fopen("ombrelloni.txt", "rw")) == NULL)
    {
        printf("Errore nell'apertura del file.\n");
        exit(-1);
    }
    else
        printf("File ombrelloni aperto correttamente.\n");

    while (!feof(f_ombrelloni))
    {
        if (fscanf(f_ombrelloni, "%d %d %d %d", &Ombrellone[i].ID, &Ombrellone[i].fila, &Ombrellone[i].numero, &Ombrellone[i].disponibile) == 4)
        {
            if (Ombrellone[i].disponibile == 0)
            {
                ombrelloni_liberi++;
            }
            i++;
        }
    }

    printf("Inserisci la frase: ");
    fgets(frase, sizeof(frase), stdin);
    Messaggio = dividiFrase(frase);
    Risposta = elaboraRisposta(ombrelloni_liberi, Messaggio, Ombrellone);
    strncpy(msg, Risposta.msg, sizeof(msg));
    printf("La frase è: %s", msg);
}