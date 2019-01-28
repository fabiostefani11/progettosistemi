#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>	
#include<time.h>
#include<signal.h>
#include<semaphore.h>
#include<pthread.h>
#include"funzioni.h"


int generaNumeri()
{
    return rand() % 10;
}

void func_BOOK(int client_sock,pthread_mutex_t mutex,Ombrellone *ombrellone,char data_inizio[20])
{
    char A[2];
    int i=0,j,z=0,t=0,conta_liberi,read_size,codnum,numero_richiesta,numero_richiesta1;
    FILE* modifiche;
    if((modifiche=fopen("aggiornamenti.txt","a"))==NULL)
        {
            printf("errore apertura file\n");
            exit(-1);
        }
    char client_message[100][2000];
    char BOOK[5]={0};
    char AVAILABLE[10]={0};
    char CANCEL[7]={0};
    char Codice[6]={0};
    char data_fine[20]={0};
    for(t=0;t<90;t++)
                {
                    if(ombrellone[t].stato==0)
                    {
                        conta_liberi++;
                    }
                }
            if(conta_liberi>0)//conta ombrelloni liberi
            {
                strcpy(client_message[i],"Prenotazione disponibile\0");
		            write(client_sock , client_message[i], strlen(client_message[i]));
					strcpy(client_message[i]," ");
                    i++;
					if((read_size = recv(client_sock , client_message[i] , 2000 , 0))>0)
                    {
						for(j=0;j<4;j++)
						{
							BOOK[j]=client_message[i][j];
						}
						BOOK[4]='\0';
						if(strcmp(BOOK,"BOOK\0")==0)
						{
							for(t=0;t<2;t++)
                			{
                    			A[t]= client_message[i][t+5];
                    
               				}
                			numero_richiesta=atoi(A);
                            numero_richiesta--;
							strcpy(client_message[i]," ");
                            i++;
                            pthread_mutex_lock(&mutex);
                            if(ombrellone[numero_richiesta].stato==0 && numero_richiesta>=-0 && numero_richiesta <=90)
                        	{
                            	ombrellone[numero_richiesta].stato=2;//ombrellone temporaneamente occupato
                            	pthread_mutex_unlock(&mutex);
                                strcpy(client_message[i],"AVAILABLE\0");
		                    	write(client_sock , client_message[i], strlen(client_message[i]));
								i++;	
                            	if((read_size = recv(client_sock , client_message[i] , 2000 , 0))>0)
								{
									for(j=0;j<4;j++)
									{
										BOOK[j]=client_message[i][j];
									}
									BOOK[4]='\0';
									for(j=0;j<6;j++)
									{
										CANCEL[j]=client_message[i][j];
									}
									CANCEL[6]='\0';
									if(strcmp(BOOK,"BOOK\0")==0)
									{

										for(t=0;t<2;t++)
                						{
                    						A[t]= client_message[i][t+5];
                    
               							}
                						numero_richiesta1=atoi(A);
                                        numero_richiesta1--;
										if(numero_richiesta==numero_richiesta1)
										{
                                            if(client_message[i][17]=='\n' || client_message[i][18]=='\n')
                                            {
                                                for(j=8;j<=18;j++)
											    {
												    data_fine[z]=client_message[i][j];
												    z++;
											    }
                                                data_fine[18]='\0';
                                                ombrellone[numero_richiesta].stato=1;
                                                strcpy(ombrellone[numero_richiesta].datainizio,data_inizio);
                                                strcpy(ombrellone[numero_richiesta].datafine,data_fine);
                                                i++;
                                                //generazione codice di cancellazione
                                                for(t=0;t<5;t++)
                                                {
                                                    codnum=generaNumeri();
                                                    Codice[t]=codnum+'0';
                                                }
                                                Codice[5]='\0';
                                                i++;
                                                strcpy(ombrellone[numero_richiesta].codice,Codice);
                                                strcpy(client_message[i],"Prenotazione avvenuta con successo\n il tuo codice di cancellazione è:  ");
                                                strcat(client_message[i],Codice);
                                                strcat(client_message[i],"\nFINE");
                                                //scrittura a file momentanea
                                                fprintf(modifiche,"%d %d %d %s %s %s\n",ombrellone[numero_richiesta].numero,ombrellone[numero_richiesta].riga,ombrellone[numero_richiesta].stato,ombrellone[numero_richiesta].codice,ombrellone[numero_richiesta].datainizio,ombrellone[numero_richiesta].datafine);
											    
											    write(client_sock ,client_message[i], strlen(client_message[i]));
                                                
                                                i++;
                                            }
                                            else
                                            {
                                                for(j=8;j<=17;j++)
											    {
												    data_inizio[z]=client_message[i][j];
												    z++;
											    }
                                                z=0;
                                                for(j=19;j<=28;j++)
											    {
												    data_fine[z]=client_message[i][j];
												    z++;
											    } 
                                                ombrellone[numero_richiesta].stato=0;
                                                strcpy(ombrellone[numero_richiesta].datainizio,data_inizio);
                                                strcpy(ombrellone[numero_richiesta].datafine,data_fine);
                                                //generazione codice
                                                for(t=0;t<5;t++)
                                                {
                                                    codnum=generaNumeri();
                                                    Codice[t]=codnum+'0';
                                                }
                                                strcpy(ombrellone[numero_richiesta].codice,Codice);
                                                strcpy(client_message[i],"Prenotazione avvenuta con successo\n il tuo codice di cancellazione è:  ");
                                                
                                                strcat(client_message[i],Codice);
                                                strcat(client_message[i],"\nFINE");
                                                //scrittura a file momentanea
                                                fprintf(modifiche,"%d %d %d %s %s %s\n",ombrellone[numero_richiesta].numero,ombrellone[numero_richiesta].riga,ombrellone[numero_richiesta].stato,ombrellone[numero_richiesta].codice,ombrellone[numero_richiesta].datainizio,ombrellone[numero_richiesta].datafine);
										
		                    				    write(client_sock , client_message[i],strlen(client_message[i]));
                                                
                                            }
											
										}
										else 
										{
											
		                    				write(client_sock ,"Hai selezionato un ombrellone diverso\nFINE",43);
                                            
										
										}
									}
									else if(strcmp(CANCEL,"CANCEL\0")==0)
									{
										ombrellone[numero_richiesta].stato=0; //ombrellone libero
		                    			write(client_sock ,"Prenotazione cancellata\nFINE", 29);
                                        
									}
                        		}
							}
                        	else
                       		{
                                pthread_mutex_unlock(&mutex);
		                    	write(client_sock ,"NAVAILABLE\nFINE", 16);
                                
                       		}	
                    	}
						else
						{
					
		                    write(client_sock ,"Inserisci BOOK e il numero dell'ombrellone che vuoi prenotare\nFINE", 67);
                           	
						}
					}
				
                /*else
                {
                   	
		            write(client_sock ,"NOK\nFINE", 9);
                    strcpy(client_message[i], "");
                    	
                } */
        }	
        else
        {
            
		    write(client_sock ,"NAVAILABLE\nFINE",16);
            
            
        }
    fclose(modifiche);
}

void func_CANCEL(int client_sock,char richiesta[2000],Ombrellone *ombrellone)
{
    char A[2];
    char cod[6];
    FILE* modifiche;
    char client_message[100][2000];
    int numero_richiesta,t,i=0,j,read_size;
    if((modifiche=fopen("aggiornamenti.txt","a"))==NULL)
        {
            printf("errore apertura file\n");
            exit(-1);
        }
        for(t=0;t<2;t++)
            {
                A[t]= richiesta[t+7];
                    
            }
        numero_richiesta=atoi(A);
        numero_richiesta--;
        if(ombrellone[numero_richiesta].stato==0 || ombrellone[numero_richiesta].stato==2 )
        {
            write(client_sock ,"L'ombrellone non può essere disdetto\nFINE",43);
        }
        else
        {
            write(client_sock ,"Inserisci il codice per disdire la prenotazione\n",49);
            if((read_size = recv(client_sock , client_message[i] , 2000 , 0))>0)
            {
                for(j=0;j<5;j++)
                {
                    cod[j]=client_message[i][j];
                }
                if((strcmp(ombrellone[numero_richiesta].codice,cod))==0)
                {
                    ombrellone[numero_richiesta].stato=0;
                    strcpy(ombrellone[numero_richiesta].codice,"00000");
                    strcpy(ombrellone[numero_richiesta].datainizio,"00/00/0000");
                    strcpy(ombrellone[numero_richiesta].datafine,"00/00/0000");

                    fprintf(modifiche,"%d %d %d %s %s %s\n",ombrellone[numero_richiesta].numero,ombrellone[numero_richiesta].riga,ombrellone[numero_richiesta].stato,ombrellone[numero_richiesta].codice,ombrellone[numero_richiesta].datainizio,ombrellone[numero_richiesta].datafine);

                    write(client_sock ,"CANCEL OK\nFINE",15);
                    
                
                }
                else
                {
                    write(client_sock ,"Codice errato\nFINE",19);
                    
                }
                    
                }
        }
        fclose(modifiche);

}


void func_AVAILABLE(int client_sock,char richiesta[2000],Ombrellone *ombrellone)
{
    int i=0,j,z,numero_richiesta;
    int conta_liberi=0,t;
    char A[2];
    char Liberi[100]={0};
    char client_message[100][2000];
    int ombr_disponibili[10]={0};

    if(richiesta[10]== '0')
            {
                for(t=0;t<90;t++)
                {
                    if(ombrellone[t].stato==0)
                    {
                        conta_liberi++;
                    }
                }
                if(conta_liberi>0)
                { 
                    strcpy(client_message[i]," ");
                    sprintf(Liberi,"%s%d",Liberi,conta_liberi);
					strcpy(client_message[i],"AVAILABLE ");
					strcat(client_message[i],Liberi);
					strcat(client_message[i],"\nFINE");
                    write(client_sock , client_message[i], strlen(client_message[i]));
                    
                    
			        i++;
                }
                else
                {
                    
		            write(client_sock ,"NAVAILABLE\nFINE",16);
                    
                   
			        
                }
            }
            else
            {
                j=0;
                for(t=0;t<2;t++)
                {
                    A[t]= richiesta[t+10];
                    
                }
                numero_richiesta=atoi(A);
                if(numero_richiesta>0 && numero_richiesta<=90)
                {
                    for(t=0;t<90;t++)
                    {
                        if(ombrellone[t].riga==numero_richiesta && ombrellone[t].stato==0)
                        {
                            ombr_disponibili[j]=ombrellone[t].numero;
                            j++;
                        }
                    }
                    strcpy(Liberi," ");
                    strcpy(client_message[i]," ");
                    strcpy(client_message[i],"Ombrelloni disponibili\n");
                    for(z=0;z<j;z++)
                    {
                        sprintf(Liberi,"%s%d",Liberi,ombr_disponibili[z]);
                        t=strlen(Liberi);
                        Liberi[t]=32;
                    }
                    strcat(client_message[i],Liberi);
					strcat(client_message[i],"\nFINE");
		            write(client_sock , client_message[i], strlen(client_message[i]));
                    
			       
                }
                else
                {
                    
		            write(client_sock ,"La fila selezionata non esiste\nFINE",36);
                    
			        
                }
    
            }

}

