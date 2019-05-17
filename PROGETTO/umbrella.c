#include "umbrella.h"
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <string.h>
#include <sys/syscall.h>
#include <unistd.h>   

pid_t gettid(void)
	{
        return syscall(SYS_gettid);
	}

int EasyRequest(umbrella *beach, int sock, char *message,tabella* tab)
{
    char *p, *c,*s;
    char output[DIM];
    char sup[100];
    int z, i = 0, control = 0;
    int num;
    //printf("--->Easy_Request\n");
    p = strtok(message, " ");
    if(strcmp(p,"iam")==0){
        sprintf(sup,"%d",(int)getpid());
        write(sock,sup,sizeof(sup));
        return 1;
    }
    if (strcmp(p, "avabile") == 0 ||
        strcmp(p, "Avabile") == 0 ||
        strcmp(p, "AVABILE") == 0)
    {
        c = p;
        c = strtok(NULL, " ");
        if (c != NULL)
        {
            num = atoi(c);
            if ((num > 0) && (num <= NROW))
            {
                z = NCOL * (num - 1);
                //printf("z:%d num:%d c:%s\n",z,num,c);
                for (z; z < num * NCOL; z++)
                {
                    if (beach[z].status == 0)
                    {
                        sprintf(sup, "%d", beach[z].number);
                        strcat(output, sup);
                    }
                    if (z < 10)
                        strcat(output, "   ");
                    if (z >= 10 && z < 100)
                        strcat(output, "  ");
                    if (z >= 100)
                        strcat(output, " ");
                }
                i = 1;
            }
        }
        else if (c == NULL)
        {
            for (z = 0; z < N_um; z++)
            {
                if (z % (NCOL) == 0)
                    strcat(output, "\n");
                if (beach[z].status == 0)
                {
                    sprintf(sup, "%d", beach[z].number);
                    strcat(output, sup);
                }
                if (beach[z].status != 0)
                {
                    if (z < 10)
                        strcat(output, " ");
                    if (z >= 10 && z < 100)
                        strcat(output, "  ");
                    if (z >= 100)
                        strcat(output, "   ");
                }
                if (z < 10)
                    strcat(output, "   ");
                if (z >= 10 && z < 100)
                    strcat(output, "  ");
                if (z >= 100)
                    strcat(output, " ");
            }
            //printf("%s", output);
            i = 1;
            //stampa Avabile+ n. ombrelloni disponibili
            // oppure n avabile
            //close_connection
        }
    }
    
    if (strcmp(p, "cancel") == 0 ||
        strcmp(p, "Cancel") == 0 ||
        strcmp(p, "CANCEL") == 0)
    {
        p = strtok(NULL, " ");
        //printf("Cancel P: %s\n", p);
        if (p == NULL)
        {   strcpy(sup,"Missing: number and ID\n");
            write(sock,sup,sizeof(sup));
            return 1;
        }
        int z = atoi(p);
        p = strtok(NULL, " ");
        if (p == NULL)
        {   strcpy(sup,"Missing: ID\n");
            write(sock,sup,sizeof(sup));
            return 1;
        }
        int id= atoi(p);

        //printf("z: %d\n status: %d\n", z, beach[z-1].status);
        if (z > 0 && id!=0 && beach[z - 1].status != 0)
        {
            beach[z - 1].status = 0;
            beach[z-1].expire=0;
            beach[z-1].start=0;
            //if(beach[z-1].abb)-------------------CANCELLA ABBONAMENTO

            //printf("------->Z: %d CANCEL OK\tStatus: %d\n",z,beach[z - 1].status);
            memset(output, 0, sizeof(output));
            strcpy(output, "Cancel ok");
            i = 1;
        }
    }
    

    if ((strcmp(p, "book") == 0 ||
         strcmp(p, "Book") == 0 ||
         strcmp(p, "BOOK") == 0))
    {   
        p =strtok(NULL," ");
        
        if (p==NULL){
            for (z = 0; z < N_um; z++)
            {
                if (beach[z].status == 0)
                {
                    control++;
                }
            }
            if (control == 0)
            {
                strcpy(output, "NAVAILABLE");
            }
            else
            {
                strcpy(output, "OK");
            }
        i = 1;
        }
    }

    if (i == 1)
    {
        //fflush(sock);
        output[strlen(output)] = 0;
        write(sock, output, strlen(output));
        memset(output, 0, DIM);
        return 1;
    }
    else
    {
        return 0;
    }
}
void *countdown(void* img){
    threadcount input=*(threadcount *)img;
    int *ptr=input.ptr;
    int num=input.num;
    struct umbrella_t *beach = input.beach;
    sleep(15);
    ptr[num]=0;
    // SEMAFORO
    if(beach[num].status!=1)beach[num].status=0;
}
int HardRequest(umbrella *beach,int sock,char *message,int *ptr,tabella* tab){
    char *p;
    int num, expire,expire1,abb=0;
    char sup[100],sup1[20];
    int i,bridge=0,fail=1;
    int g=0;
    //printf("%s\n",message);
    char zz[100];
    strcpy(zz,message);
    p=strtok(zz," ");
    memset(sup1,0,sizeof(sup1));
        while (p!=NULL){   
            g++;
            if (g>5){
                strcat(sup1,p);
                strcat(sup1," ");
            }
            p=strtok(NULL," ");
            if(g==8)abb=1;
        }
    
    p=strtok(message," ");//elimino book
    
    p=strtok(NULL," ");// num ombrellone
    num=atoi(p);
    num--;
    
    p=strtok(NULL," ");// data fine
    if(p==NULL && g<=2){
        if(ptr[num]==0 && beach[num].status!=1){
            if(beach[num].status==0){
                ///------------------SEMAFORIIIIIII____-------
            ptr[num]=(int)gettid();
            beach[num].status=2;
            //---------------------------
            pthread_t thread_id;
            threadcount img;
            img.beach=beach;
            img.num=num;
            img.ptr=ptr;
            pthread_create(&thread_id, NULL, countdown,(void *)&img);
            
            sprintf(sup,"temporarily Booked N: %d, ID: %d\nYou have 45 second to confirm.\n",num+1,(int)gettid());
            //countDOWN____
            write(sock,sup,strlen(sup)); 
            fail=0;   
            }
        }
        else{
            sprintf(sup,"Not avabile FOR TODAY");
            if(beach[num].status==2)strcat(sup," at the moment");
            if(beach[num].status==2 && (ptr[num]==(int)gettid())) sprintf(sup,"You have to complete the Booking process\n");
            strcat(sup,"\n");
            write(sock,sup,strlen(sup));
            fail=0; 
        }
        

    }
    else{//// IF con g, g______________________
        if(ptr[num]==gettid()){
            memset(sup,0,sizeof(sup));
            for(i=0;i<3;i++){
                strcat(sup,p);
                if(i<2){
                    strcat(sup," ");
                    p=strtok(NULL," ");
                }
            }           
            expire=getdate(sup);//------------ DATA 1
            
            if(abb!=1){
                printf("------->Expire: %d\n",expire);            
                

                if(beach[num].expire==0 && expire!=-1){//----book da oggi
                    beach[num].expire=expire;
                    beach[num].status=1;
                    beach[num].diritto=ptr[num];
                    
                    sprintf(sup,"Booked confirmed, N: %d, ID: %d\tStatus: %d,Expire: %d\n",num+1,ptr[num],beach[num].status,beach[num].expire);
                    write(sock,sup,strlen(sup));
                    fail=0;             
                }
            }
            if(abb==1){
                printf("------->SECOND DATE: ->%s<-\n",sup1);
                expire1=getdate(sup1);
                sprintf(sup,"Abbonamento, da finire :)\n");
                umbrella *toInsert=(umbrella*)malloc(sizeof(umbrella));;
                toInsert->start=expire;
                toInsert->expire=expire1;
                if(toInsert->start < GetSysDate()){
                    toInsert->start=GetSysDate();
                    bridge=1;
                }
                if(toInsert->expire < GetSysDate()){
                    sprintf(sup,"NOT BOOKED, expire in the past\n");
                    write(sock,sup,strlen(sup)); 
                    return 0;
                }
                toInsert->number=(num)+1;
                toInsert->diritto=ptr[num];
                if(insert(tab,toInsert)){
                    toInsert->status=1;
                    beach[num].abb=1;
                    if(bridge==1){
                        beach[num].start=toInsert->start;
                        beach[num].expire=toInsert->expire;
                        beach[num].status=toInsert->status;
                        beach[num].diritto=toInsert->diritto;
                    }
                    sprintf(sup,"ABBONAMENTO confirmed\n\tN: %d, ID: %d\tStatus: %d,Expire: %d\n",num+1,ptr[num],toInsert->status,toInsert->expire);
                    write(sock,sup,strlen(sup));
                    
                }                
                fail=0;
                toInsert=NULL;
            }
        }

        if(fail){
            sprintf(sup,"NOT BOOKED, something goes wrong :(\n");
            write(sock,sup,strlen(sup)); 
        }
    }
    //p=strtok(NULL," ");// data inizio-fine -> abbonamento
    
}
int getdate(char* input){
    int date, i, dd, mm, yyyy;
    i = 0;
    char *c = strtok(input, " ");
    do
    {
        switch (i)
        {
        case (0):
            dd = atoi(c);
            if (dd < 1 || dd > 31)
            {
                i = -1;
            }
            break;
        case (1):
            mm = atoi(c);
            if (mm < 1 || mm > 12)
            {
                i = -1;
            }
            break;
        case (2):
            yyyy = atoi(c);
            if (yyyy < 1900 || yyyy > 2100)
            {
                i = -1;
            }
            break;
        }
        i++;
        if (i == 0)
        {   
            return -1;
        }
        if (i > 0)
            c = strtok(NULL, " ");
    } while (c != NULL && i<3);
    date = dd + (mm * 100) + (yyyy * 10000);
    //printf("date: %d\n", date);
    return date;
}
int GetSysDate(){
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    char sup[20];
    int date;
    sprintf(sup,"%d %d %d\n",tm.tm_mday,tm.tm_mon + 1,tm.tm_year + 1900);
    date=getdate(sup);

    return date;
}
void pretab(tabella* tabella){
    int i;
    
    for(i=0;i<N_um;i++){
        nodo* z=(nodo*)malloc(sizeof(nodo));
        z->next=NULL;
        z->prev=NULL;
        //umbrella *toInsert=(umbrella*)malloc(sizeof(umbrella));
        z->el=NULL;//(umbrella*)malloc(sizeof(umbrella));
	    tabella->um_number[i]=z;	
    }
}
int insert(tabella* tabella,umbrella* elemento){
    //printf("-->ok start\n");
    int pos=(elemento->number)-1;
    nodo* element=(nodo*)malloc(sizeof(nodo));
        element->next=NULL;
        element->prev=NULL;
        element->el=elemento;
    //printf("-->ok elemento\n"); 
    int finish,start=0;    
    nodo* now=(nodo*)malloc(sizeof(nodo));
        now->next=NULL;
        now->prev=NULL;
        now->el=NULL;
    //printf("-->ok nodo NOW\n-->pos: %d\texpire previsto: %d\n",pos,element->el->expire);
    //now->el=tabella->um_number[pos]->el;
    //printf("--> tabella[%d]: %d\n",pos,tabella->um_number[pos]->el->expire);      
    if(tabella->um_number[pos]->el==NULL){ //elemento vuoto, inserisci in testa
        //printf("-->ok el vuoto...\n");
        element->prev=NULL;
        element->next=NULL; 
        tabella->um_number[pos]=element;
        //printf("-> Inserimento cella vuota\n");
        return 1;       
    }
    //printf("-->zzzzzzzzzzzz\n");
    if(tabella->um_number[pos]!=NULL){// inserimento ordinato
        start=element->el->start;
        finish=element->el->expire;
        now=tabella->um_number[pos];
        //printf("-> Start: %d\n",start);
    
        if(finish < now->el->start && now->next ==NULL){ //Inizio
          //  printf("Ok All' Inizio\n");
            now->prev=element;
            element->next=now;
            element->prev=NULL;            
            tabella->um_number[pos]=element;
            return 1;
        }
        while((start > now->el->expire)){ //coda
            if(now->next == NULL){
            //    printf("Ok In coda\n");
                element->prev=now;
                now->next=element;
                element->next=NULL;
              //  printf("-> Inserimento In coda\n");
                return 1;
            }
            if(finish < now->next->el->start){
                //printf("Ok in mezzo\n");
                now->next->prev=element;
                element->next=now->next;
                now->next=element;
                element->prev=now;                
                
                return 1;
            }
            now=now->next;                                  
        }
        return 0;
        printf("DIO PORCO\n");
        
    }
      
}