#include <stdio.h>

#define PROTOPORT 8888 //numero della porta di default
#define QLEN 3         //grandezza della coda
#define BUFFERSIZE 256
#define RED "\x1b[31m"
#define CRESET "\x1b[0m"

#define DIM 256
#define dim 10

typedef struct
{
    int ID;
    int numero;
    int fila;
    int disponibile; //0=disponibile 1=occupato adesso 2=occupato in futuro 3=occupato sia adesso che in futuro 4=temporaneamente occupato
    int IDclient;
    int data_inizio;
    int data_fine;
} ombrellone;

typedef struct
{
    char parola[DIM];
    int IDCLient;
} aggiornamento;

typedef struct nodo
{
    ombrellone dato;
    struct nodo *next;
} Nodo;

typedef Nodo *lista;

typedef struct
{
    char parola[DIM];
    int ID;
    int ombrellone;
    int fila;
    int data_inizio;
    int data_fine;
    int nparole;
    int IDclient;

} messaggio;

typedef struct
{
    ombrellone Ombrellone[DIM];
    char msg[DIM];
    int ombrelloni_liberi;
    int IDclient;
    lista lista;
    int ombrelloni_Toccupati[101];
} risposta;

int uniscidata(char data[]);
messaggio dividiFrase(char msg[]);
char *elaboraRisposta(risposta *Risposta, messaggio Messaggio);
void crealista(lista *l);
void inserimento(lista *l, int ID, int fila, int numero, int IDclient, int data_inizio, int data_fine);
int confrontoDate(int inizioPrenotazione, int finePrenotazione, int inizioRichiesta, int fineRichiesta);
int ricerca(lista *l, int ID, int datainizio, int datafine);
void elimTesta(lista *l);
int eliminaPrenotazione(lista *l, int IDclient, int fila, int numero, int data_inizio);
void stampaListaSuFile(lista *l, FILE *f);
aggiornamento dividiAggiornamento(char msg[]);
char *ricercaAvailableNumero(lista *l, int datainizio, int datafine);
char *ricercaAvailable(lista *l, int fila, int datainizio, int datafine);