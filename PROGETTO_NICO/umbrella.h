#define NROW 14
#define NCOL 12
#define ANSI_COLOR_CYAN "\x1b[36m"
#define ANSI_COLOR_RESET "\x1b[0m"
#define N_um 132
#define DIM 3000
#include <pthread.h>
typedef struct umbrella_t
{
  int number;
  int status; //0-free 1-occuped 2-temp_occupied
  int start;
  int expire;
  int abb;
  int diritto; // Diritto di accedere all' ombrellone (X cancel)
} umbrella;
typedef struct threadinput_t
{
  int sockfd;
  struct umbrella_t *beach;
  struct chain_t *tab;
} threadinput;

typedef struct threadcount_t
{
  int num;
  int *ptr;
  struct umbrella_t *beach;
} threadcount;

typedef struct nodo_t
{
  umbrella *el;
  struct nodo_t *next;
  struct nodo_t *prev;
} nodo;

typedef struct chain_t
{
  nodo *um_number[N_um];
} tabella;

void PrintStatus(void *);
void TxtStatus(umbrella[]);
umbrella *BookInput(char *, umbrella *);
void readBeachStatus(void *);
int EasyRequest(umbrella *, int, char *, tabella *);
pid_t gettid(void);
int HardRequest(umbrella *, int, char *, int *, tabella *);
void *countdown(void *img);
int getdate(char *);
int getdateStart();
void pretab(tabella *);
int insert(tabella *, umbrella *);
int GetSysDate();