#include<stdio.h>	
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<signal.h>

int go=1;
int sock;
int flag=0;

int main(int argc, char **argv)
{
	int s_lengt,lunghezza_messaggio;
	int i=0,j,t,flag=0;
	struct sockaddr_in server;
	char message[100][1000];

	if(argc== 1 || argc==4 || argc==5)
	{
		printf("Numero di argomenti incorretto\n");
		exit(-1);
	}
	sock = socket(AF_INET , SOCK_STREAM , 0);
	if (sock == -1)
	{
		printf("Errore creazione socket");
	}
	puts("Socket created");
	
	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	server.sin_family = AF_INET;
	server.sin_port = htons( 8888 );

	if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
	{
		perror("Connessione fallita. Errore");
		return 1;
	}
	puts("Connesso\n");
		
	strcpy(message[i],argv[1]);
	if((strcmp(message[i],"AVAILABLE")==0)&&  argc==2 )
	{
		t=strlen(message[i]);
		message[i][t]=' ';
		strcat(message[i],"0");
	}
	if(argc==3)
	{
		t=strlen(message[i]);
		message[i][t]=' ';
		strcat(message[i],argv[2]);
	}
	
		if( send(sock , message[i], strlen(message[i]) , 0) < 0)
		{
			puts("invio messaggio fallito");
			return 1;
		}
		i++;
		if( recv(sock , message[i] , 2000 , 0) < 0)
		{
			puts("ricezione messaggio fallita");
			exit(3);
		}
			lunghezza_messaggio=strlen(message[i]);
			if(message[i][lunghezza_messaggio-5]=='\n' && message[i][lunghezza_messaggio-4]=='F')
		{
			flag++;
			printf("Server: ");
			for(j=0;j<(lunghezza_messaggio-4);j++)
			{
				printf("%c",message[i][j]);
			}
			exit(4);
		}
		else
		{
			printf("Server: %s \n",message[i]);
		}
		i++;
	while(go)
	{
		if(flag==0)
		{
			printf("Inserisci richiesta : ");
			fgets(message[i],40,stdin);
		if( send(sock , message[i], strlen(message[i]) , 0) < 0)
		{
			puts("invio messaggio fallito");
			return 1;
		}

		}

		if( recv(sock , message[i] , 2000 , 0) < 0)
		{
			puts("ricezione messaggio fallita");
			break;
		}
			lunghezza_messaggio=strlen(message[i]);
			if(message[i][lunghezza_messaggio-5]=='\n' && message[i][lunghezza_messaggio-4]=='F')
		{
			flag++;
			printf("Server: ");
			for(j=0;j<(lunghezza_messaggio-4);j++)
			{
				printf("%c",message[i][j]);
			}
			break;
		}
		else
		{
			printf("Server: %s \n",message[i]);
		}
		i++;
	}
	return 0;
}



