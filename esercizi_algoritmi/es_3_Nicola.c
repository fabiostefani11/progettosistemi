#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <limits.h>

#define N 1000      // dimensione massima dell'array da ordinare
#define STEP 100    // gap tra due esperimenti successivi
#define NUMEXP 1000 // numero di volte che un esperimento di una certa dimensione si ripete

struct tipo_heap
{
    int *vettore;
    int heapsize;
};
typedef struct tipo_heap heap;

int isOrdered(int *, int);
void Stampa(int *, int);
int left(int);
int right(int);
int parent(int);

void MaxHeapify(heap *, int);
double HeapSort(int *, heap *, int);
void BuildMaxHeap(heap *);

void MaxHeapifyIterativo(heap *, int);
double HeapSortIterativo(int *, heap *, int);
void BuildMaxHeapIterativo(heap *);

int Partition(int *, int, int);
void QuickSort(int *, int, int);

void MergeSort(int *, int, int);
void Merge(int *, int, int, int);

void scambia(int *, int *);           // scambia il valore di due interi
void GeneraVettore(int *, int);       // genera un vettore casuale di una determinata dimensione
void CopiaVettore(int *, int *, int); // copia un vettore in un altro
double ArrToHeap(int *, heap *, int);

int main()
{
    clock_t start, end; // variabili per la tempistica
    int dimensione;     // dimensione del vettore attuale durante l'esperimento
    int iterazione;     // indice dell'iterazione durante l'esperimento

    int *vettore;
    int *vettore_copia;
    double QS, MS, HS, HSP, HIS, HISP;          // variabili per mantenere la sommma dei tempi dei tre algoritmi
    double mediaQS, mediaMS, mediaHS, mediaHIS; // variabili per le medie dei tempi (al variare dell'iterazione)

    heap *h;
    h = malloc(sizeof(int) * N);
    (*h).vettore = malloc(sizeof(int) * N);
    FILE *risultati;

    srand(1); // seme per i numeri casuali (NON USARE 'time')

    vettore = malloc(sizeof(int) * N);
    vettore_copia = malloc(sizeof(int) * N);

    risultati = fopen("risultati.txt", "w");
    if (risultati == NULL)
    {
        printf("ERRORE nell'apertura del file");
        return (1);
    }

    printf("Dimensione\tQuickSort\tMergeSort\tHeapSort\tHeapSortIterativo\n");
    fprintf(risultati, "Dimensione\tQuickSort\tMergeSort\tHeapSort\tHeapSortIterativo\n");
    for (dimensione = STEP; dimensione <= N; dimensione += STEP)
    {
        QS = 0;
        MS = 0;
        HS = 0;
        HIS = 0;
        HISP = 0;
        for (iterazione = 1; iterazione <= NUMEXP; iterazione++)
        {
            GeneraVettore(vettore, dimensione);
            CopiaVettore(vettore, vettore_copia, dimensione);

            start = clock();
            QuickSort(vettore, 0, dimensione - 1);
            end = clock();
            QS = QS + (double)(end - start) / CLOCKS_PER_SEC;

            CopiaVettore(vettore_copia, vettore, dimensione);

            start = clock();
            MergeSort(vettore, 0, dimensione - 1);
            end = clock();
            MS = MS + (double)(end - start) / CLOCKS_PER_SEC;

            CopiaVettore(vettore_copia, vettore, dimensione);

            start = clock();
            HSP = HeapSort(vettore, h, dimensione);
            end = clock();
            HS = HS + ((double)(end - start) / CLOCKS_PER_SEC) - HSP;

            /*Stampa(h->vettore,dimensione);
            printf("-----------------------\n");
            */
            CopiaVettore(vettore_copia, vettore, dimensione);

            start = clock();
            HISP = HeapSortIterativo(vettore, h, dimensione);
            end = clock();
            HIS = HIS + ((double)(end - start) / CLOCKS_PER_SEC) - HISP;
            //Stampa(h->vettore,dimensione);
        }
        mediaQS = QS / NUMEXP;
        mediaMS = MS / NUMEXP;
        mediaHS = HS / NUMEXP;
        mediaHIS = HIS / NUMEXP;
        printf("%d\t %f\t %f\t %f\t %f \n", dimensione, mediaQS, mediaMS, mediaHS, mediaHIS);
        fprintf(risultati, "%d\t %f\t %f\t %f\t %f\n", dimensione, mediaQS, mediaMS, mediaHS, mediaHIS);
    }

    fclose(risultati);
}

//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------

int Partition(int *arr, int primo, int ultimo)
{
    int pivot = arr[ultimo]; // pivot
    int i = (primo - 1);     // Indice elemento più piccolo
    int j;
    for (j = primo; j <= ultimo - 1; j++)
    {

        if (arr[j] <= pivot) // Se elemento corrente è più piccolo o uguale al pivot
        {
            i++; // incrementa indice elemento più piccolo
            scambia(&arr[i], &arr[j]);
        }
    }
    scambia(&arr[i + 1], &arr[ultimo]);
    return (i + 1);
}

void QuickSort(int *arr, int primo, int ultimo)
{
    if (primo < ultimo)
    {
        // pi indice partizione, arr[primo] è al posto giusto
        int pi = Partition(arr, primo, ultimo);

        QuickSort(arr, primo, pi - 1); //Sort separato elementi prima e dopo pivot
        QuickSort(arr, pi + 1, ultimo);
    }
}

void MergeSort(int *arr, int sx, int dx)
{
    if (sx < dx)
    {

        int centro = sx + (dx - sx) / 2;

        MergeSort(arr, sx, centro);
        MergeSort(arr, centro + 1, dx);

        Merge(arr, sx, centro, dx);
    }
    return;
}

void Merge(int *arr, int sx, int centro, int dx)
{
    int i, j, k;
    int n1 = centro - sx + 1;
    int n2 = dx - centro;

    int SX[n1], DX[n2]; //array temporanei

    for (i = 0; i < n1; i++)
        SX[i] = arr[sx + i];
    for (j = 0; j < n2; j++)
        DX[j] = arr[centro + 1 + j]; //copio in DX e SX array di partenza
    //----------------------------------------------------------------------
    i = 0;                   // Indice Sx
    j = 0;                   // Indice Dx
    k = sx;                  // Indice di partenza
    while (i < n1 && j < n2) //copio DX e SX in array di partenza(ora ordinato)
    {
        if (SX[i] <= DX[j])
        {
            arr[k] = SX[i];
            i++;
        }
        else
        {
            arr[k] = DX[j];
            j++;
        }
        k++;
    }
    //----------------------------------------------------------------------
    // Copia elementi rimanenti di SX[], se ci sono
    while (i < n1)
    {
        arr[k] = SX[i];
        i++;
        k++;
    }

    // Copia elementi rimanenti di DX[], se ci sono
    while (j < n2)
    {
        arr[k] = DX[j];
        j++;
        k++;
    }
    return;
}
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------

void GeneraVettore(int *vettore, int dimensione)
{
    int i;
    int casuale;

    for (i = 0; i < dimensione; i++)
    {
        casuale = rand() % dimensione;
        vettore[i] = casuale;
    }
}
void CopiaVettore(int *vettore_origine, int *vettore_destinazione, int dimensione)
{
    int i;

    for (i = 0; i < dimensione; i++)
    {
        vettore_destinazione[i] = vettore_origine[i];
    }
}
void scambia(int *a, int *b)
{
    int temp;
    temp = *a;
    *a = *b;
    *b = temp;
}
void Stampa(int *vettore, int dimensione)
{
    int i;

    for (i = 0; i < dimensione; i++)
        printf("vettore[%d]: %d\n", i, vettore[i]);
}
int isOrdered(int *vettore, int dimensione)
{
    int i;
    int Ordered = 1;

    for (i = 0; i < dimensione - 1; i++)
    {
        if (vettore[i] >= vettore[i + 1])
        {
            Ordered = 0;
            break;
        }
    }
    return Ordered;
}
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------

// Funzioni Heap Sort
int left(int i)
{
    return (2 * i) + 1;
}
int right(int i)
{
    return (2 * i + 1) + 1;
}
int parent(int i)
{
    return i / 2;
}
double ArrToHeap(int *arr, heap *h, int dimensione) //restituisce tempo impiegato conversione arr-> heap
{
    clock_t p1, p2;
    double HSP = 0;
    int i;

    p1 = clock();
    h->heapsize = dimensione;

    for (i = 0; i < dimensione; i++)
    {
        h->vettore[i] = arr[i];
    }
    p2 = clock();
    HSP = (double)(p2 - p1) / CLOCKS_PER_SEC;
    return HSP;
}
//--  --  --  --  --  --  --  --
//--  --  --  HEAP SORT ITERATIVO
//--  --  --  --  --  --  --  --
void MaxHeapifyIterativo(heap *h, int i)
{
    int dx, sx, max;

    //for(i; i<h->heapsize; i++)
    max = i;
    do
    {
        i = max;
        sx = left(i);
        dx = right(i);
        if ((sx <= h->heapsize) && (h->vettore[sx] > h->vettore[i]))
        {
            max = sx;
        }
        else
        {
            max = i;
        }
        if ((dx <= h->heapsize) && (h->vettore[dx] > h->vettore[max]))
        {
            max = dx;
        }
        if (max != i)
        {
            scambia(&h->vettore[i], &h->vettore[max]);
        }
    } while (max != i);
}
void BuildMaxHeapIterativo(heap *h)
{
    int i;
    for (i = h->heapsize / 2; i > 0; i--)
    {
        MaxHeapifyIterativo(h, i);
    }
}
double HeapSortIterativo(int *vettore, heap *h, int dim)
{

    int i;
    double HSP;
    HSP = ArrToHeap(vettore, h, dim);
    BuildMaxHeapIterativo(h);
    for (i = dim; i > 1; i--)
    {
        scambia(&h->vettore[1], &h->vettore[i]);
        h->heapsize = h->heapsize - 1;
        MaxHeapifyIterativo(h, 1);
    }
    return HSP;
}
//--  --  --  --  --  --  --  --
//--  --  --  HEAP SORT   --  --
//--  --  --  --  --  --  --  --
void MaxHeapify(heap *h, int i)
{
    int dx, sx, max;
    sx = left(i);
    dx = right(i);
    if (sx <= h->heapsize && h->vettore[sx] > h->vettore[i])
    {
        max = sx;
    }
    else
    {
        max = i;
    }
    if (dx <= h->heapsize && h->vettore[dx] > h->vettore[max])
    {
        max = dx;
    }
    if (max != i)
    {
        scambia(&h->vettore[i], &h->vettore[max]);
        MaxHeapify(h, max);
    }
}

void BuildMaxHeap(heap *h)
{
    int i;
    for (i = h->heapsize / 2; i > 0; i--)
    {
        MaxHeapify(h, i);
    }
}
double HeapSort(int *vettore, heap *h, int dim)
{

    int i;
    double HSP;
    HSP = ArrToHeap(vettore, h, dim);
    BuildMaxHeap(h);
    for (i = dim; i > 1; i--)
    {
        scambia(&h->vettore[1], &h->vettore[i]);
        h->heapsize = h->heapsize - 1;
        MaxHeapify(h, 1);
    }
    return HSP;
}
