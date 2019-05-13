all: server client 

server: server.o funzioniServer.o
	gcc -g -o server server.o funzioniServer.o -lpthread

server.o: server.h server.c
	gcc -g -c server.c

funzioniServer.o: funzioniServer.c
	gcc -g -c funzioniServer.c

client: client.o funzioniClient.o
	gcc -g -o client client.o funzioniClient.o

client.o: client.h client.c
	gcc -g -c client.c

funzioniClient.o: funzioniClient.c
	gcc -g -c funzioniClient.c