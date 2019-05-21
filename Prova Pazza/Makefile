all: server client 

server: server.o funzioniServer.o 
	gcc -g -o server server.o funzioniServer.o -lpthread -lrt

server.o: server.h server.c
	gcc -g -c server.c

funzioniServer.o: funzioniServer.c
	gcc -g -c funzioniServer.c

client: client.o
	gcc -g -o client client.o

client.o: client.c
	gcc -g -c client.c

