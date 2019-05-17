#include <stdio.h>
#include <string.h> //strlen
#include <stdlib.h> //strlen
#include <sys/socket.h>
#include <arpa/inet.h> //inet_addr
#include <unistd.h>    //write
#include <pthread.h>
#include <signal.h>
#include <sys/stat.h>
#include <syslog.h>
#include <semaphore.h>
#include "umbrella.h"


#define K 101

sem_t SlotLibero, SlotOccupato, mutex;
void *connection_handler(void *);
void *connection_master(void *);
//int EasyRequest(umbrella * , int , char * );
void sighand(int);
int i;
int date;
int wp = 0;
int rp = 0;
char buffer[K];
int go = 1;
int tempID[N_um];
int main(int argc, char *argv[])
{
    pid_t myself;
    struct umbrella_t beach[N_um];
    tabella* tab=(tabella*)malloc(sizeof(tabella));
    pretab(tab); 
    sem_init(&SlotLibero, 1, K);
    sem_init(&SlotOccupato, 1, 0);
    sem_init(&mutex, 1, 0);
    pthread_t thread_id, masterThr;
    int socket_desc, client_sock, c;
    struct sockaddr_in server, client;
    struct sigaction sa;
    int f;
    //sigemptyset(&sa);
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = sighand;
    
    date=GetSysDate();
    printf("Date: %d\n",date);

    myself = fork();

    if (myself != 0)
    {
        exit(0);
    }
    if (setsid() < 0)
        exit(EXIT_FAILURE);

    sigaction(SIGINT, &sa, 0);
    sigaction(SIGTERM, &sa, 0);
    sigaction(SIGHUP, &sa, 0);
    sigaction(SIGQUIT, &sa, 0);

    /* Fork off for the second time*/
    myself = fork();

    /* An error occurred */
    if (myself < 0)
        exit(EXIT_FAILURE);

    /* Success: Let the parent terminate */
    if (myself > 0)
        exit(EXIT_SUCCESS);

    /* Set new file permissions
    https://stackoverflow.com/questions/17954432/creating-a-daemon-in-linux/17955149#17955149
    !!change directory and permessi 
    */
    usleep(500);
    printf("\nson SERVER PID: %d\n Father PID: %d\n", (int)getpid(), (int)getppid());
    for(f=0;f<N_um;f++){
        tempID[f]=0;
    }
    if (pthread_create(&masterThr, NULL, connection_master, (void *)&beach) != 0)
    {
        perror("Thread master creation failed. Error");
        return 0; 
    }
    puts("--->Thread Master created");
    sem_wait(&SlotOccupato);
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_desc == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(8888);

    //Bind
    if (bind(socket_desc, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        //print the error message
        perror("bind failed. Error");
        return 1;
    }
    puts("bind done");

    //Listen
    listen(socket_desc, 3);

    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);

    struct threadinput_t thin;

    while ((client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t *)&c)))
    {
        thin.beach = beach;
        thin.tab=tab;
        thin.sockfd = client_sock;
        puts("Connection accepted");

        if (pthread_create(&thread_id, NULL, connection_handler, (void *)&thin) < 0)
        {
            perror("could not create thread");
            return 1;
        }

        puts("Handler assigned");
        i++;
        printf("\tClient connected: %d\n", i);
    }

    if (client_sock < 0)
    {
        perror("accept failed");
        return 1;
    }

    return 0;
}
void *connection_master(void *beach1)
{
    sem_wait(&SlotLibero);
    puts("\tReading Beach Status..");
    readBeachStatus(beach1);
    puts("\tRead success");
    //PrintStatus(beach1);
    //journaling da beach_new.txt a beach_status.txt o beach_abb.txt
    sem_post(&SlotOccupato);
}
void *connection_handler(void *thin)
{
    //Get the socket descriptor
    threadinput input = *(threadinput *)thin;
    struct umbrella_t *beach = input.beach;
    struct chain_t *tab= input.tab;
    int sock = input.sockfd;
    int read_size;
    pthread_t self = pthread_self();
    char messagecpy[DIM], client_message[DIM];
    int close = 0;
    //Receive a message from client
    while ((read_size = recv(sock, client_message, DIM, 0)) > 0)
    {
        //---------------------------------------------------------------------
        client_message[read_size] = '\0';
        //printf("Recieved_Message: -%s-\n", client_message);

        //---------------------------------------------------------------------
        //end of string marker
        if (strcmp(client_message, "quit") == 0)
        {
            printf("Exiting...\n--->ABBONAMENTI:\n");
            nodo* t=(nodo*)malloc(sizeof(nodo));
        //----------------------DELETEEEEE
            for(i=0;i<N_um;i++){
                if(tab->um_number[i]->el!=NULL){
                    t=tab->um_number[i];
                    printf("Num: %d\tSTART\t - EXPIRE\n",t->el->number);
                    while(t->next!=NULL){
                        printf("\t%d - %d \n",t->el->start,t->el->expire); 
                        t=t->next;
                    }
                    printf("\t%d - %d\n",t->el->start,t->el->expire);                 
                }
        
            }
            sighand(2);
        }
        strcpy(messagecpy,client_message);
        if (!EasyRequest(beach, sock, messagecpy,tab))
        {
            // !!! CREARE BOOK !!!
            //----------------------------------------------pid_t se=syscall(_NR__gettid);

            //printf("--->Hard_Request\n");
            HardRequest(beach,sock,client_message,tempID,tab);
            //Send the message back to client
            //write(sock, client_message, strlen(client_message));
            //clear the message buffer
            memset(client_message, 0, DIM);
        }
    }
    if (go == 0)
    {   //--------------------------------
        
        //--------------------------------
        
        //write(sock , NULL , 0);
        shutdown(sock, SHUT_RDWR);
    }

    if (read_size == 0)
    {
        puts("Client disconnected");
        i--;
        printf("\tClient connected: %d\n", i);

        fflush(stdout);
    }
    else if (read_size == -1)
    {
        perror("recv failed");
    }

    return 0;
}

void readBeachStatus(void *beach1)
{
    struct umbrella_t *beach = (struct umbrella_t *)beach1;
    char del[] = " ";
    FILE *fd;
    int num = 0;
    char c[30];
    char sup[30];
    int i = 0,gg;
    char *p,*q;

    if ((fd = fopen("spiaggia.txt", "r")) == NULL)
    {
        perror("Errore apertura");
        exit(-1);
    }
    puts("\tINIT open correctly!");
    while (fgets(c, 30, fd) != NULL)
    {
        p = strtok(c, del);
        while (p)
        {
            if (i == 0)
            {
                beach[num].number = (atoi(p));
            }
            else if (i == 1)
            {
                beach[num].status = atoi(p);
            }
            else if (i == 2)
            {
                memset(sup,0,sizeof(sup));
                    if(atoi(p)!=0){
                        q=p;
                    for(i=0;i<3;i++){ 
                        strcat(sup,q);
                        if(i<2){
                            strcat(sup," ");//\0");
                            
                            q=strtok(NULL," ");
                                                       
                        }
                    }         
                    gg=getdate(sup);
                    if(atoi(p)==0)gg=0;
                    if((date>=gg)&&beach[num].status==1)beach[num].start=date;
                    else{
                        beach[num].start==0;
                    }
                    }
            }
            else if (i == 3)
                {   memset(sup,0,sizeof(sup));
                    if(atoi(p)!=0){
                        q=p;
                    for(i=0;i<3;i++){
                        //printf("-->P: %s",q);  
                        strcat(sup,q);
                        //sup[sizeof(sup)]='\0';
                        //snprintf(sup,sizeof(&p),"%s%s",q,p);
                        if(i<2){
                            strcat(sup," ");//\0");
                            
                            q=strtok(NULL," ");
                            //printf("-->P: %s, I: %d\n",p,i);                            
                        }
                        //printf("\tI: %d\tSUP: ->%s<-\n",i,sup);
                    }         
                    gg=getdate(sup);
                    //printf("\nN: %d GG: -%d-\n",beach[num].number,gg);
                    }
                    if(atoi(p)==0)gg=0;
                    if(gg<date && beach[num].status==1)i=-2;
                    else{
                        i=3;
                        beach[num].expire = gg;
                    }
                }
            p = strtok(NULL, del);
            i++;
            if(i==-1){
                beach[num].start=0;
                beach[num].expire=0;
                beach[num].status=0;
                i=4;
            }
            if (i == 4)
            {
                i = 0;
                num++;
            }
        }
    }
    if (fclose(fd) != 0)
    {
        perror("Errore chiusura file\n");
        exit(-2);
    }
}
void PrintStatus(void *beach1)
{
    struct umbrella_t *beach = (struct umbrella_t *)beach1;
    int n_um = 0, r, c;

    printf(">>Stato Spiaggia:\n");
    for (r = 0; r < NROW; r++)
    {
        if (r == 0)
            printf("\n");
        for (c = 0; c < NCOL; c++)
        {
            if (c == 0)
                printf("\t");
            if (n_um < N_um)
            {
                if (beach[n_um].status)
                {
                    printf(ANSI_COLOR_CYAN "%d \t" ANSI_COLOR_RESET, beach[n_um].number);
                }
                else
                    printf("%d \t", beach[n_um].number);
            }
            n_um++;
        }
        printf("\n");
    }
}
void sighand(int sig)
{
    printf("\n");
    go = 0;
    if (sig == SIGINT)
    {
        printf("SIGINT received ... GAME OVER ! \n");
    }
    if (sig == SIGTERM)
    {
        printf("SIGTERM received ... GAME OVER ! \n");
    }
    if (sig == SIGQUIT)
    {
        printf("SIGQUIT received ... GAME OVER ! \n");
    }
    if (sig == SIGHUP)
    {
        printf("SIGHUP received ... GAME OVER ! \n");
    }

    
    exit(0);
}
int getdateStart()
{
    int date, i, dd, mm, yyyy;
    char input[20];
    printf("Insert today date: (dd mm yyyy)\n");
    fgets(input, 20, stdin);
    if (strchr(input, '\n') == NULL)
    { //check exist newline
        printf("\nOverflow, closing\n");
        exit(0);
    }
    do{
        date=getdate(input);
        if(date==-1){
            printf("Invalid, %d %d %d insert again: (dd mm yyyy)\n", dd, mm, yyyy);
            fgets(input, 20, stdin);
        }
    }while(date==-1);      
    return date;
}

