#include <stdio.h>

int main()
{
    char str[50];

    printf("Inserisci frase: ");
    fgets(str);

    printf("Hai scritto: %s\n", str);

    return (0);
}