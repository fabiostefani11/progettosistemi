#include <stdio.h>
#include <string.h>

int main()
{
    char numero[6];
    printf("Scrivi qualcosa: ");
    fgets(numero, sizeof(numero), stdin);
    printf("il numero è: %s\n", numero);

    if (1 != 0)
    {
        char numero[6] = "ciao";
        printf("il numero è: %s\n", numero);
    }
    printf("il numero è: %s\n", numero);
}