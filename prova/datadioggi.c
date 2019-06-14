#include <stdio.h>
#include <time.h>

int main()
{
    time_t data;
    int giorno, mese, anno;
    time(&data);
    struct tm *local = localtime(&data);
    giorno = local->tm_mday;
    mese = local->tm_mon + 1;
    anno = local->tm_year + 1900;
    char *dataodierna;
    sprintf(dataodierna,"%02d/%02d/%d\n", giorno, mese, anno);
    printf("%s",dataodierna);
}