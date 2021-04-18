/**
Titlul temei: Să se realizeze un program de utilizare a funcției execve() pentru executarea de comenzi shell.
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

	if (argc < 2) {
		printf("Utilizare: ./04 <binar>\n");
		exit(-1);
	}

	char* const envVar[] = {"PATH=CEVA:CE:S-A:INLOCUIT", "CUSTOM_VAR=NISTE:VALORI:LA:INTAMPLARE", NULL};

	printf("PROCES APELANT:: proces ID=%ld, parinte ID= %ld\n", (long)getpid(), (long)getppid());

	execve(argv[1], &argv[1], envVar);

	perror("Apelul functiei execve a esuat!: ");
	exit(-1);

	
}

/**
1)
$ gcc lab03t04.c -o 04
./04
Utilizare: ./03 <binar>

2)
$ gcc lab03t04.c -o 04
$ ./04 ps
PROCES APELANT:: proces ID=15250, parinte ID= 9166
Apelul functiei execve a esuat!: : No such file or directory

3)
$ gcc lab03t04.c -o 04
$ ./04 /bin/ps -f
PROCES APELANT:: proces ID=15252, parinte ID= 9166
UID        PID  PPID  C STIME TTY          TIME CMD
alext     9166  9156  0 18:24 pts/0    00:00:02 bash
alext    15252  9166  0 21:08 pts/0    00:00:00 /bin/ps -f

4)
$ gcc lab03t04.c -o 04
$ ./04 ./echo_ 1 2 3
PROCES APELANT:: proces ID=15255, parinte ID= 9166
	PROCES ECHO:: proces ID=15255, parinte ID= 9166
	PROCES ECHO:: argv[0]: ./echo_
	PROCES ECHO:: argv[1]: 1
	PROCES ECHO:: argv[2]: 2
	PROCES ECHO:: argv[3]: 3
	PROCES ECHO:: PATH= CEVA:CE:S-A:INLOCUIT
	PROCES ECHO:: CUSTOM_VAR= NISTE:VALORI:LA:INTAMPLARE
**/