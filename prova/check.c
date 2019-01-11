#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main()
{
    FILE* f;
    int fila, nombrellone, disponibile;
    int ombrellone;
    int nfila;

    if((f=fopen("ombrelloni.txt","r"))==NULL)
    {
        printf("errore apertura file\n");
        exit(-1);
    }
    else 
    printf("file aperto\n");

    printf("quale fila vuole ?\n");
    scanf("%d",&nfila);
    printf("che ombrellone vuoi prenotare ?\n");
    scanf("%d", &ombrellone);

    while(!feof(f))
    {
        if(fscanf(f,"%d%d%d",&fila,&nombrellone,&disponibile)==3)
        if(fila==nfila)
        {
            if(nombrellone==ombrellone)
            {
            if(disponibile)
            printf("available\n");
            else 
            printf("notavailable\n");
            }

        }

    }
    fclose(f);
}