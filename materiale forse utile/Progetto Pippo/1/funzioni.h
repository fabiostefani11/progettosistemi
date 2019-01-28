

typedef struct
{
    int numero;
    int riga;
    int colonna;
    int stato;   //0:libero 1:occupato 2:temporaneamente occupato
    char codice[6];
    char datainizio[20]; 
    char datafine[20];
}Ombrellone;

typedef struct
{
    char Data[20];
    int sock;
}parametri;


void func_BOOK(int client_sock,pthread_mutex_t mutex,Ombrellone *ombrellone,char data_inizio[20]);
void func_CANCEL(int client_sock,char richiesta[2000],Ombrellone *ombrellone);
void func_AVAILABLE(int client_sock,char richiesta[2000],Ombrellone *ombrellone);

