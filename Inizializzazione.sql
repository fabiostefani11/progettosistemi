SET foreign_key_checks=0;
CREATE TABLE LIBRO ( 
	ID_LIBRO 	VARCHAR(15) NOT NULL,
	TITOLO 	VARCHAR(15) NOT NULL,
	EDITORE 	VARCHAR(15) NOT NULL, 	-- NOT NULL significa che DEVE avere un valore
			
	PRIMARY KEY (ID_LIBRO) 					-- Definisco la chiave primaria (PK)
	);

CREATE TABLE AUTORI ( 
	NOME_AUTORE 			VARCHAR(15) NOT NULL,
	ID_LIBRO 		VARCHAR(15) NOT NULL,
	
	PRIMARY KEY (NOME_AUTORE),
	FOREIGN KEY (ID_LIBRO) REFERENCES LIBRO(ID_LIBRO)
    ); 


CREATE TABLE EDITORE ( 
	NOME_EDITORE 		VARCHAR(15) NOT NULL,
	INDIRIZZO_EDITORE			VARCHAR(15) NOT NULL,
    TELEFONO			VARCHAR(10) NOT NULL,
	
	PRIMARY KEY (NOME_EDITORE)					-- La PK è composta da 2 campi
	);

CREATE TABLE COPIE_LIBRO ( 
	ID_LIBRO 		VARCHAR(15) NOT NULL,
	ID_BIBLIOTECA 	VARCHAR(15) NOT NULL,
	NUMERO_COPIE 		INT(3) NOT NULL,
    
	FOREIGN KEY (ID_LIBRO) REFERENCES LIBRO(ID_LIBRO),
    FOREIGN KEY (ID_BIBLIOTECA) REFERENCES BIBLIOTECA(ID_BIBLIOTECA)
);

CREATE TABLE PRESTITI (
	ID_LIBRO 	VARCHAR(15) NOT NULL,
    ID_BIBLIOTECA 	VARCHAR(15) NOT NULL,
    N_CARTA		INT(10) NOT NULL,
    DATA_INIZIO	DATE NOT NULL,
    DATA_FINE	DATE NOT NULL,
    
    FOREIGN KEY (ID_LIBRO) REFERENCES LIBRO(ID_LIBRO),
    FOREIGN KEY (ID_BIBLIOTECA) REFERENCES BIBLIOTECA(ID_BIBLIOTECA),
    FOREIGN KEY (N_CARTA) REFERENCES CLIENTE(N_CARTA)
    
);

CREATE TABLE BIBLIOTECA ( 
	ID_BIBLIOTECA 	VARCHAR(15) NOT NULL,
	NOME_SEZIONE 	VARCHAR(15) NOT NULL,
	INDIRIZZO_SEZIONE			VARCHAR(15) NOT NULL,
	
	PRIMARY KEY (ID_BIBLIOTECA)
); 

CREATE TABLE CLIENTE (
	N_CARTA		INT(10) NOT NULL,
    NOME_CLIENTE VARCHAR(15) NOT NULL,
    INDIRIZZO_CLIENTE VARCHAR(15) NOT NULL,
    TELEFONO		VARCHAR(10) NOT NULL,
    
    PRIMARY KEY (N_CARTA)
);
SET foreign_key_checks=1;
