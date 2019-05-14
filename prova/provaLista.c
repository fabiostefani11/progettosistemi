#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <malloc.h>

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

typedef struct prenotazione
{
    int fila;
    int ombrellone;
    int datainizio;
    int datafine;
} prenotazione;

typedef struct nodo
{
    ombrellone dato;
    struct nodo *next;
} Nodo;

typedef Nodo *lista;

void crealista(lista *l)
{
    *l = NULL;
}

void inserimento(lista *l, int ID, int fila, int numero, int IDclient, int data_inizio, int data_fine)
{
    Nodo *aux = (Nodo *)malloc(sizeof(Nodo));
    aux->dato.ID = ID;
    aux->dato.fila = fila;
    aux->dato.numero = numero;
    aux->dato.IDclient = IDclient;
    aux->dato.data_inizio = data_inizio;
    aux->dato.data_fine = data_fine;
    aux->next = *l;
    *l = aux;
}

void stampa(lista *l)
{
    while (*l)
    {
        printf("%d %d %d %d %d %d \n", (*l)->dato.ID, (*l)->dato.fila, (*l)->dato.numero, (*l)->dato.IDclient, (*l)->dato.data_inizio, (*l)->dato.data_fine);
        l = &(*l)->next;
    }
}

void stampaListaSuFile(lista *l, FILE *f)
{
    while (*l)
    {
        fprintf(f, "%d %d %d %d %d %d \n", (*l)->dato.ID, (*l)->dato.fila, (*l)->dato.numero, (*l)->dato.IDclient, (*l)->dato.data_inizio, (*l)->dato.data_fine);
        l = &(*l)->next;
    }
}

int confrontoDate(int inizioPrenotazione, int finePrenotazione, int inizioRichiesta, int fineRichiesta)
{
    int trovato = 0;

    //CASO 2
    if ((inizioRichiesta >= inizioPrenotazione) && (fineRichiesta <= finePrenotazione))
    {
        trovato = 1;
    }

    //CASO 1
    if ((inizioRichiesta <= inizioPrenotazione) && (fineRichiesta >= inizioPrenotazione) && (fineRichiesta <= finePrenotazione))
    {
        trovato = 1;
    }

    //CASO 3
    if ((inizioRichiesta <= finePrenotazione) && (inizioRichiesta >= inizioPrenotazione) && (fineRichiesta >= finePrenotazione))
    {
        trovato = 1;
    }

    //CASO 4
    if ((inizioRichiesta <= inizioPrenotazione) && (fineRichiesta >= finePrenotazione))
    {
        trovato = 1;
    }

    return trovato;
}

void elimTesta(lista *l)
{
    Nodo *aux = *l;
    *l = (*l)->next;
    free(aux);
}

lista *ricerca(lista *l, int ID, int datainizio, int datafine)
{
    int trovato = 0;
    while (*l)
    {
        if ((*l)->dato.ID == ID)
        {
            if (confrontoDate((*l)->dato.data_inizio, (*l)->dato.data_fine, datainizio, datafine) == 1)
            {
                trovato = 1;
                break;
            }
        }
        l = &(*l)->next;
    }
    if (trovato == 0)
    {

        printf("L'ombrellone NON è occupato in questo periodo\n");
    }
    else
        printf("L'ombrellone è occupato in questo periodo\n");
}

lista *eliminaPrenotazione(lista *l, int IDclient, int fila, int numero)
{
    int trovato = 0;
    while (*l)
    {
        if (((*l)->dato.IDclient == IDclient) && ((*l)->dato.ID == ((fila * 10) + numero) - 10))
        {
            elimTesta(l);
            trovato = 1;
            break;
        }

        l = &(*l)->next;
    }
    if (trovato == 1)
    {

        printf("La prenotazione è stata eliminata\n");
    }
    else
        printf("Prenotazione non trovata\n");
}

int main()
{
    FILE *f_ombrelloni;

    if ((f_ombrelloni = fopen("ombrelloniLista.txt", "r+")) == NULL)
    {
        printf("Errore nell'apertura del file.\n");
        exit(-1);
    }
    else
        printf("File ombrelloni aperto correttamente.\n");

    lista l;
    prenotazione p;
    int ID, fila, numero, IDclient, data_inizio, data_fine;

    crealista(&l);

    while (!feof(f_ombrelloni))
    {
        if (fscanf(f_ombrelloni, "%d %d %d %d %d %d", &ID, &fila, &numero, &IDclient, &data_inizio, &data_fine) == 6)
        {
            inserimento(&l, ID, fila, numero, IDclient, data_inizio, data_fine);
        }
    }
    fclose(f_ombrelloni);

    //stampa(l);

    int IDnuovo = 0;
    int IDCanc = 0;
    int filaCanc, numeroCanc;
    int conferma = 0;

    printf("fila: ");
    scanf("%d", &p.fila);
    printf("ombrellone: ");
    scanf("%d", &p.ombrellone);
    printf("data inizio: ");
    scanf("%d", &p.datainizio);
    printf("datafine: ");
    scanf("%d", &p.datafine);
    ricerca(&l, ((p.fila * 10) + p.ombrellone) - 10, p.datainizio, p.datafine);

    printf("Inserisci 1 per confermare la prenotazione, 2 per cancellare una prenotazione, 0 per terminare:");
    scanf("%d", &conferma);

    if (conferma == 1)
    {

        printf("Per confermare la prenotazione digitare l'ID:\n");
        scanf("%d", &IDnuovo);
        inserimento(&l, ((p.fila * 10) + p.ombrellone) - 10, p.fila, p.ombrellone, IDnuovo, p.datainizio, p.datafine);
        printf("Prenotazione confermata! \n");
    }

    if (conferma == 2)
    {
        printf("Per eliminare una prenotazione digitare l'IDclient: ");
        scanf("%d", &IDCanc);
        printf("fila: ");
        scanf("%d", &filaCanc);
        printf("ombrellone: ");
        scanf("%d", &numeroCanc);

        eliminaPrenotazione(&l, IDCanc, filaCanc, numeroCanc);
    }

    if ((f_ombrelloni = fopen("ombrelloniLista.txt", "w")) == NULL)
    {
        printf("Errore nell'apertura del file.\n");
        exit(-1);
    }
    else
        printf("File ombrelloni aperto correttamente.\n");

    stampaListaSuFile(&l, f_ombrelloni);
    stampa(&l);

    fclose(f_ombrelloni);
}