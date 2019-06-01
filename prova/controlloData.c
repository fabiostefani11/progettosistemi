#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DIM 256

int uniscidata(char *data)
{
    char giorno[DIM] = {0};
    char mese[DIM] = {0};
    char anno[DIM] = {0};

    strcpy(giorno, strtok(data, "/"));
    strcpy(mese, strtok(NULL, "/"));
    strcpy(anno, strtok(NULL, "/"));

    strcat(anno, mese);
    strcat(anno, giorno);

    return atoi(anno);
}

int controlloData(int giorno, int mese, int anno)
{
    if (giorno < 0 || mese < 0 || anno < 0 || giorno > 31 || mese > 12)
    {
        return 0;
    }
    if (mese == 2)
    {
        if ((anno % 400 == 0 || anno % 4 == 0 && anno % 100 != 0) && (giorno > 29))
        {
            return 0;
        }
        else if (giorno > 28)
        {
            return 0;
        }
    }
    if ((mese == 4) || (mese == 6) || ((mese == 9)) || (mese == 11))
    {
        if (giorno > 30)
        {
            return 0;
        }
    }
    if ((mese == 1) || (mese == 3) || (mese == 5) || (mese == 7) || (mese == 8) || (mese == 10) || (mese == 12))
    {
        if (giorno > 31)
        {
            return 0;
        }
    }

    return 1;
}

int main()
{

    int giorno, mese, anno;
    char *data_1 = (char *)malloc(sizeof(char) * 11);
    char *data_2 = (char *)malloc(sizeof(char) * 11);
    ;
    int data_fine;
    int i = 0;
    printf("Inserisci una data: ");
    scanf("%s", data_1);

    strcpy(data_2, data_1);

    char *GIORNO, *MESE, *ANNO;

    char *token;
    token = strtok(data_1, "/");
    while (token != NULL)
    {
        if (i == 0)
        {
            GIORNO = token;
        }
        if (i == 1)
        {
            MESE = token;
        }
        if (i == 2)
        {
            ANNO = token;
        }
        token = strtok(NULL, "/");

        i++;
    }

    giorno = atoi(GIORNO);
    mese = atoi(MESE);
    anno = atoi(ANNO);

    if (controlloData(giorno, mese, anno) == 0)
    {
        printf("Data inesistente.\n");
    }
    else
    {
        data_fine = uniscidata(data_2);
        printf("la data finale è: %d\n", data_fine);
    }
}