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
    int IDclient;

} messaggio;

typedef struct
{
    int ID;
    int numero;
    int fila;
    int disponibile; //0=disponibile 1=occupato adesso 2=occupato in futuro 3=occupato sia adesso che in futuro 4=temporaneamente occupato
    int IDclient;
} ombrellone;

typedef struct
{
    ombrellone Ombrellone[DIM];
    char msg[DIM];
    int ombrelloni_liberi;
    int IDclient;
} risposta;

int uniscidata(char data[]);
messaggio dividiFrase(char msg[]);
risposta elaboraRisposta(risposta Risposta, messaggio Messaggio);