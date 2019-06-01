#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DIM 256

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

int uniscidata(char data[])
{
    int giorno, mese, anno;
    int data_fine;
    int i = 0;

    char *GIORNO, *MESE, *ANNO;

    char *token;
    token = strtok(data, "/");
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
        data_fine = -1;
    }
    else
    {
        strcat(ANNO, MESE);
        strcat(ANNO, GIORNO);
        data_fine = atoi(ANNO);
    }

    return data_fine;
}

int main()
{
    char data[DIM] = {0};
    printf("Inserisci una data: ");
    scanf("%s", data);

    printf("la data finale è: %d\n", uniscidata(data));
}