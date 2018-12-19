provaClient: provaClient.o funzioniClient.o
	gcc -g -o funzioniClient provaClient.o funzioniClient.o

provaClient.o: provaClient.c librerieClient.h
	gcc -g -c provaClient.c

funzioniClient.o: funzioniClient.c librerieClient.h
	gcc -g -c funzioniClient.c