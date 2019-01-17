#define PROTOPORT 5193 //numero della porta di default
#define QLEN 6         //grandezza della coda
#define BUFFERSIZE 256

#define DIM 256
#define dim 10

typedef struct
{
    char parola[DIM];
    int ombrellone;
    int fila;
    int data_inizio;
    int data_fine;
    int nparole;

} messaggio;

int uniscidata(char data[]);
messaggio dividiFrase(char msg[]);
char confrontaParola(char parola[]);