/**
Titlul temei: Să se realizeze un program de utilizare a funcției execle() pentru executarea de comenzi shell.
Data livrarii: 14.03.2021

Student: Alexandru Iulian Ticu
An 3 IA (recontractare)

<Se va folosi binarul compilat dupa fisierul lab03echo.c pentru a demonstra functionalitatea programului curent.>

**/
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv){

	printf("PROCES APELANT:: proces ID=%ld, parinte ID= %ld\n", (long)getpid(), (long)getppid());

	char* const envVar[] = {"CUSTOM_VAR=NISTE:VALORI:LA:INTAMPLARE", NULL};

	printf("PROCES APELANT:: PATH= %s\nPROCES APELANT:: CUSTOM_VAR= %s\n", getenv("PATH") == NULL ? "" : getenv("PATH"), getenv("CUSTOM_VAR") == NULL ? "" : getenv("CUSTOM_VAR"));

	execle("./echo_", "Primul argument", "Al doilea argument", NULL, envVar);
	perror("Apelul functiei execle a esuat!: ");
	exit(-1);
}

/**

1) Se poate observa cum variabila PATH nu este mostenita, chiar daca aceasta exista in programul apelant.
$ gcc lab03t02.c -o 02
$ gcc lab03echo.c -o echo_
$ ./02 
PROCES APELANT:: proces ID=15505, parinte ID= 9166
PROCES APELANT:: PATH= /home/alext/.local/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/usr/local/games:/snap/bin
PROCES APELANT:: CUSTOM_VAR= 
	PROCES ECHO:: proces ID=15505, parinte ID= 9166
	PROCES ECHO:: argv[0]: Primul argument
	PROCES ECHO:: argv[1]: Al doilea argument
	PROCES ECHO:: PATH= 
	PROCES ECHO:: CUSTOM_VAR= NISTE:VALORI:LA:INTAMPLARE

**/