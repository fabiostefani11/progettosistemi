#include <stdio.h>

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
    printf("Inserisci frase parola/ombrellone/fila/data/data_fine: ");
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

    printf("La parola è: %s", Messaggio.parola);
    printf("L'ombrellone è: %s", Messaggio.ombrellone);
    printf("La fila è: %s", Messaggio.fila);
    printf("La data è: %s", Messaggio.data);
    printf("La data di fine è: %s", Messaggio.data_fine);
}