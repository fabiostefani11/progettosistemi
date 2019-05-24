#include <stdio.h>

#define PROTOPORT 8888 //numero della porta di default
#define QLEN 6         //grandezza della coda
#define BUFFERSIZE 256

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
int eliminaPrenotazione(lista *l, int IDclient, int fila, int numero);
void stampaListaSuFile(lista *l, FILE *f);
void aggiornaFile(risposta *Risposta, int ombrellone_attuale, FILE *f_ombrelloni, FILE *f_prenotazioni);