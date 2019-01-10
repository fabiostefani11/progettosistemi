#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    char parola[10];
    char ombrellone[3];
    char fila[3];
    char data[9];
    char data_fine[9];

} messaggio;

int main()
{
    messaggio Messaggio;
    char msg[256] = {0};
    int i = 0;
    int k = 0;
    FILE *f;
    fgets(msg, sizeof(msg), stdin);

    while (msg[i] != ' ' || msg[i] != '\0')
    {
        Messaggio.parola[k] = msg[k];
        k++;
        i++;
    }
    i++;
    k = 0;
    while (msg[i] != ' ' || msg[i] != '\0')
    {
        Messaggio.ombrellone[k] = msg[i];
        i++;
        k++;
    }
    i++;
    k = 0;
    while (msg[i] != ' ' || msg[i] != '\0')
    {
        Messaggio.fila[k] = msg[i];
        i++;
        k++;
    }
    i++;
    k = 0;
    while (msg[i] != ' ' || msg[i] != '\0')
    {
        Messaggio.data[k] = msg[i];
        i++;
        k++;
    }
    i++;
    k = 0;
    while (msg[i] != ' ' || msg[i] != '\0')
    {
        Messaggio.data_fine[k] = msg[i];
        i++;
        k++;
    }

    if ((f = fopen("ombrelloni.txt", "r")) == NULL)
    {
        printf("Errore apertura file\n");
        exit(-1);
    }

    while (!trovato && !feof(f))
    {
        if (fscanf(f, "%s%s%s", nome, indirizzo, numero) == 3)
            if (strcmp(nome, nome_ricerca) == 0)
            {
                printf("%s\n%s\n%s\n", nome, indirizzo, numero);
                trovato = 1;
            }
    }

    if (!trovato)
    {
        printf("%s non trovato\n", nome_ricerca);
    }

    if (fclose(f))
    {
        printf("Errore chiusura file\n");
        exit(-2);
    }
    return 0;
}