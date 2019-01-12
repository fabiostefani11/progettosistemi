#define PROTOPORT 5193 //numero della porta di default
#define QLEN 6         //grandezza della coda
#define BUFFERSIZE 256

#define DIM 256

typedef struct
{
    char parola[DIM];
    char ombrellone_str[DIM];
    char fila_str[DIM];
    char data_inizio_str[DIM];
    char data_fine_str[DIM];
    int ombrellone;
    int fila;
    int data_inizio;
    int data_fine;

} messaggio;

int uniscidata(char data[]);
char confrontaParola(char parola[]);