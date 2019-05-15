all: server client 

server: server.o funzioniServer.o thpool.o
	gcc -g -o server server.o funzioniServer.o thpool.o -lpthread -lrt

server.o: server.h server.c thpool.h
	gcc -g -c server.c

funzioniServer.o: funzioniServer.c
	gcc -g -c funzioniServer.c

client: client.o funzioniClient.o
	gcc -g -o client client.o funzioniClient.o

client.o: client.h client.c
	gcc -g -c client.c

funzioniClient.o: funzioniClient.c
	gcc -g -c funzioniClient.c

thpool.o: thpool.h thpool.c
	gcc -g -c thpool.c 