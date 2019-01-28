#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define CRESET  "\x1b[0m"


int main()
{
    FILE* f;
    int fila, nombrellone, disponibile;
    int ombrellone;
    int nfila;

    if((f=fopen("ombrelloni.txt","r"))==NULL)
    {
        printf( RED "errore apertura file\n" CRESET);
        exit(-1);
    }
    else 
    printf( GREEN "file aperto\n" CRESET);

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
            printf(GREEN "available\n" CRESET);
            else 
            printf(RED "notavailable\n" CRESET);
            }

        }

    }
    fclose(f);
}