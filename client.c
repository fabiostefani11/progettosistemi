// per semplicità ho scritto tutto in un file 
   

#include <stdio.h>
#include <stdlib.h>
// per l'uso delle socket
#include <sys/socket.h>  
// per le operazioni di internet ( assegnazione ip/porta)         
#include <arpa/inet.h>


int main(int argc, char* argv[])
{   
    // creazione socket 
    int sock;
    sock=socket(PF_INET,SOCK_STREAM,0);
    if(sock<0)
    {
         printf("creazione socket fallita \n");
         close(sock);
    }
    else printf("socket creata \n");

    // costruzione indirizzo del server 
    struct sockaddr_in sad;
    // setto i valori della struttura a 0
    memset(&sad,0,sizeof(sad));

    sad.sin_family=AF_INET;
    // ip del server 
    sad.sin_addr.s_addr=inet_addr("127.0.0.1");
    // porta del server 
    /* htons assicura che i numeri siano memorizzati 
       in memoria nell'ordine dei byte di rete;
       i protocolli che usiamo richiedono proprio questo ordine */
    sad.sin_port=htons(5193);

    // connessione al server 
    connect(sock,(struct sockaddr *)&sad,sizeof(sad));
    if(connect<0)
    {
        printf("Connessione fallita");
        close(sock);
    }
    else printf("Connessione riuscita \n");

    // invio dati al server 
    char* messaggio="prova invio";
    int stringlen=strlen(messaggio);
    send(sock,messaggio,stringlen,0);
    if(send!=stringlen)
    {
        printf("L'invio non si è concluso correttamente \n");
        close(sock);
    }
    else printf("Invio riuscito \n");


    // ricevere dati dal server (da scrivere)

    


    //chiusura socket
    close(sock);
}
