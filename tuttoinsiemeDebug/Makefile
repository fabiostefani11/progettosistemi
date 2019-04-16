all: client serverDebug

serverDebug: serverDebug.c
	gcc -g -o serverDebug serverDebug.c

client: client.o funzioniClient.o
	gcc -g -o client client.o funzioniClient.o

client.o: client.h client.c
	gcc -g -c client.c

funzioniClient.o: funzioniClient.c
	gcc -g -c funzioniClient.c