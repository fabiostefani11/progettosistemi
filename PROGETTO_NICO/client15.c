#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#define __USE_MISC
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <netdb.h>
#include <pthread.h>
#include <semaphore.h>
#include "umbrella.h"

void *connection_check(void *);
sem_t mux;

int main(int argc, char *argv[])
{
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    int read_size;
    char buffer[DIM];
    char test[DIM];
    //----------------------------
    char *p, *c;
    char del[] = " \n";
    int i = 0;
    int ex = 0;
    pthread_t thread_id;
    sem_init(&mux, 0, 1);
    //-----------------------------
    if (argc < 3)
    {
        fprintf(stderr, "usage %s hostname port\n", argv[0]);
        exit(0);
    }
    portno = atoi(argv[2]);
    /* Create a socket point */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("ERROR opening socket");
        exit(1);
    }
    server = gethostbyname(argv[1]);
    if (server == NULL)
    {
        fprintf(stderr, "ERROR, no such host\n");
        exit(0);
    }

    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno);

    /* Now connect to the server */
    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) //-------------------!!!!!! CAST SOCK ADDR !!!!
    {
        perror("ERROR connecting");
        exit(1);
    }
    if (pthread_create(&thread_id, NULL, connection_check, (void *)&sockfd) < 0)
    {
        perror("could not create thread");
        return 1;
    }

    /* Send message to the server */
    while (1)
    {
        //-------------------------------------------READ FROM BUFFER

        n = 0;
        memset(buffer, 0, DIM);
        char *p;
        do
        {
            printf("INPUT:\n");
            if (n = strlen(fgets(buffer, DIM, stdin)))
            {
                if (strchr(buffer, '\n') == NULL)
                { //check exist newline
                    printf("\nBuffer overflow, closing\n");
                    exit(-2);
                }
                if (n == 1 || n == 0)
                    i = 101;
                strcpy(test, buffer);
                p = strtok(test, del);
                do
                {
                    if (i > 0)
                        i++;
                    if (i == 0)
                    {
                        if (strcmp(p, "BOOK") == 0 ||
                            strcmp(p, "Book") == 0 ||
                            strcmp(p, "book") == 0 ||
                            strcmp(p, "CANCEL") == 0 ||
                            strcmp(p, "Cancel") == 0 ||
                            strcmp(p, "cancel") == 0 ||
                            strcmp(p, "AVABILE") == 0 ||
                            strcmp(p, "Avabile") == 0 ||
                            strcmp(p, "avabile") == 0 ||
                            strcmp(p, "quit") == 0 ||
                            strcmp(p, "iam") == 0)
                        {
                            i++;
                        }
                    }
                    p = strtok(NULL, del);
                } while (p != NULL && i > 0);

                if (i == 0 || i > 8)
                {
                    printf("Invalid input insert again:\n");
                    n = 0;
                    i = 0;
                }
            }
        } while (n == 0 || n == 1);
        buffer[strcspn(buffer, "\n")] = 0;
        //printf("I'm gonna send: -%s-\n", buffer);

        n = write(sockfd, buffer, strlen(buffer));
        i = 0;
        if (n < 0)
        {
            perror("ERROR writing to socket");
            exit(1);
        }
        sem_wait(&mux);
        if ((read_size = recv(sockfd, buffer, DIM, 0)) > 0)
        {
            buffer[read_size] = 0;
            //printf("server says:\n\t->%s<-\nCharacters read: %d\n", buffer, read_size); //-------------------------------------------READ FROM BUFFER
            printf("%s\n", buffer);
            memset(buffer, 0, DIM);
        }
        sem_post(&mux);
    }

    return 0;
}
void *connection_check(void *socket_desc)
{
    //Get the socket descriptor
    int sock = *(int *)socket_desc;
    int read_size;
    char data;
    while (1)
    {
        sem_wait(&mux);
        if (recv(sock, &data, 1, MSG_PEEK) == 0)
        {
            printf("Server is down\n");
            exit(0);
        }
        sem_post(&mux);
        sleep(1);
    }
}
