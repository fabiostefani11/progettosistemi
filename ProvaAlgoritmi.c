#include <stdio.h>
#include <stdlib.h>
#define DIM 5

void StampaArray(int *, int);
void GeneraVettore(int *, int);
//INSERISCI NOMI DELLE FUNZIONI Es: "int Partition(int *, int, int);
//                                   void QuickSort(int *, int, int);"

int main()
{
    int i, *vettore;
    vettore = malloc(sizeof(int) * DIM);
    GeneraVettore(vettore, DIM);
    printf("Stampa dell'array NON ordinato: \n");
    StampaArray(vettore, DIM);
    //INSERISCI ALGORITMO es: "QuickSort(vettore, 0, DIM - 1);"
    printf("Stampa dell'array ordinato: \n");
    StampaArray(vettore, DIM);
}
void GeneraVettore(int *vettore, int dimensione)
{
    int elemento;

    printf("Inserisci un array di %d elementi: \n", DIM);
    for (int i = 0; i < dimensione; i++)
    {
        scanf("%d", &elemento);
        vettore[i] = elemento;
    }
    printf("\n");
}
void StampaArray(int *vettore, int dimensione)
{
    for (int i = 0; i < dimensione; i++)
        printf("%d ", vettore[i]);
}

/*INSERISCI CORPO DELLE FUNZIONI Es:
int Partition(int *vettore, int p, int r)
{
    ..........
}
*/