provaServer: provaServer.o funzioniServer.o
	gcc -g -o funzioniServer provaServer.o funzioniServer.o

provaServer.o: provaServer.c librerieServer.h
	gcc -g -c provaServer.c

funzioniServer.o: funzioniServer.c librerieServer.h
	gcc -g -c funzioniServer.c