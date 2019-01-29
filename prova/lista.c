#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#define RED "\x1b[31m"
#define GREEN "\x1b[32m"
#define CRESET "\x1b[0m"

typedef struct prenotazione
{
    int fila;
    int ombrellone;
    int datainizio;
    int datafine;
} prenotazione;

typedef struct Ombrellone
{

    int id;
    int datainizio;
    int datafine;

} Ombrellone;

typedef struct nodo
{
    Ombrellone dato;
    struct nodo *next;
} Nodo;

typedef Nodo *lista;

void crealista(lista *l)
{
    *l = NULL;
}

void inserimento(lista *l, int id, int datainizio, int datafine)
{
    Nodo *aux = (Nodo *)malloc(sizeof(Nodo));
    aux->dato.id = id;
    aux->dato.datainizio = datainizio;
    aux->dato.datafine = datafine;
    aux->next = *l;
    *l = aux;
}

lista *ricerca(lista *l, int id, int datainizio, int datafine)
{
    int trovato = 0;
    while (*l)
    {
        if ((*l)->dato.id == id)
        {
            if ((*l)->dato.datainizio == datainizio)
            {
                trovato = 1;
                break;
            }
        }
    }
    if (trovato == 0)
    {

        inserimento(l, id, datainizio, datafine);
    }
    else
        printf(RED "prenotazione già trovata\n" CRESET);
}

void stampa(lista l)
{
    while (l)
    {
        printf("%d\n%d\n%d\n", l->dato.id, l->dato.datainizio, l->dato.datafine);
        l = l->next;
    }
}

int main()
{
    lista l;
    crealista(&l);
    prenotazione p;
    int i;
    for (i = 0; i <= 2; i++)
    {
        printf("fila: ");
        scanf("%d", &p.fila);
        printf("ombrellone: ");
        scanf("%d", &p.ombrellone);
        printf("data inizio: ");
        scanf("%d", &p.datainizio);
        printf("datafine: ");
        scanf("%d", &p.datafine);
        ricerca(&l, ((p.fila * 10) + p.ombrellone) - 10, p.datainizio, p.datafine);
    }
    stampa(l);
}