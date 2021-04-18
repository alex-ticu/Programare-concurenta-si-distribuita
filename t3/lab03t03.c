/**
Titlul temei: Să se realizeze un program de utilizare a funcției execv() pentru executarea de comenzi shell.
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
		printf("Utilizare: ./03 <binar>\n");
		exit(-1);
	}

	printf("PROCES APELANT:: proces ID=%ld, parinte ID= %ld\n", (long)getpid(), (long)getppid());

	execv(argv[1], &argv[1]);

	perror("Apelul functiei execv a esuat!: ");
	exit(-1);

}

/**
1) Se apeleaza ps -f. Comanda esueaza pentru ca execv cauta doar in directoarele cu "/" sau in directorul curent.
$ gcc lab03t03.c -o 03
./03 ps -f
PROCES APELANT:: proces ID=14732, parinte ID= 9166
Apelul functiei execv a esuat!: : No such file or directory

2) Se apeleaza /bin/ps -f. Comanda se executa pentru ca se da calea exacta catre binar.
$ gcc lab03t03.c -o 03
$ ./03 /bin/ps -f
PROCES APELANT:: proces ID=14756, parinte ID= 9166
UID        PID  PPID  C STIME TTY          TIME CMD
alext     9166  9156  0 18:24 pts/0    00:00:01 bash
alext    14756  9166  0 20:45 pts/0    00:00:00 /bin/ps -f

3) Se observa faptul ca in acest caz, programul echo printeaza "argv[0]: echo_". Acest comportament este datorat de faptul ca in apelul functiei execv se transmite adresa &argv[1], ce poitneaza spre un vector. Prima valoare din acest vector este chiar argv[1], care in cazul executabilului ps se ignora.
$ gcc lab03t03.c -o 03
$ gcc lab03echo.c -o echo_
$ ./03 echo_ 1 2 3
PROCES APELANT:: proces ID=14776, parinte ID= 9166
	PROCES ECHO:: proces ID=14776, parinte ID= 9166
	PROCES ECHO:: argv[0]: echo_
	PROCES ECHO:: argv[1]: 1
	PROCES ECHO:: argv[2]: 2
	PROCES ECHO:: argv[3]: 3
	PROCES ECHO:: PATH= /home/alext/.local/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/usr/local/games:/snap/bin
	PROCES ECHO:: CUSTOM_VAR=
**/