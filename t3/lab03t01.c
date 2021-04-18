/**
Titlul temei: Să se realizeze un program de utilizare a funcției execlp() pentru executarea de comenzi shell.
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

	execlp("echo_", "Primul argument", "Al doilea argument", NULL);
	perror("Apelul \"echo_ Primul argument Al doilea argument\" a esuat");
	exit(-1);
}


/**

1) Daca binarul echo_ nu este intr-un director din PATH, atunci se printeaza o eroare.
$ gcc lab03t01.c -o 01
$ ./01 
PROCES APELANT:: proces ID=12116, parinte ID= 9166
Apelul "echo_ Primul argument Al doilea argument" a esuat: No such file or directory

2) Se copiaza binarul echo_ in primul director din PATH (la mine este este /home/alext/.local/bin/) si se executa din nou programul. De data aceasta functioneaza.
$ gcc lab03t01.c -o 01
$ gcc lab03echo.c -o echo_
$ cp echo_ `echo ${PATH} | awk -F ':' '{print $1}'`  // Incearca sa copieze binarul echo_ in primul director din PATH
$ if [[ $? -ne 0 ]]; then sudo cp echo_ `echo ${PATH} | awk -F ':' '{print $1}'`; fi // daca comanda esueaza, atunci se presupune ca utilizatorul curent nu are permisiune asupra directorului si se foloseste comanda sudo.
$ ./01

PROCES APELANT:: proces ID=12730, parinte ID= 9166
PROCES ECHO:: proces ID=12730, parinte ID= 9166
argv[0]: Primul argument	// Primul argument nu este ./echo_ .
argv[1]: Al doilea argument
PATH= /home/alext/.local/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/usr/local/games:/snap/bin 	// Variabila de mediu este mostenita.
CUSTOM_VAR=

$ rm `echo ${PATH} | awk -F ':' '{print $1}'`/echo_  // Se va sterge binarul copiat anterior.
$ if [[ $? -ne 0 ]]; then sudo rm `echo ${PATH} | awk -F ':' '{print $1}'`/echo_; fi // In caz de esec se presupune ca utilizatorul nu are permisiune asupra directorului si se foloseste comanda sudo.
**/