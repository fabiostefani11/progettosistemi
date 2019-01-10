#include <stdio.h>

int main()
{
    char str[50];

    printf("Inserisci frase: ");
    fgets(str, sizeof(str), stdin);

    printf("Hai scritto: %s", str);

    return (0);
}