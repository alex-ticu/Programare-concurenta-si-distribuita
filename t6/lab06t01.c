/**
Titlul temei: Realizarea unui token-ring unidirecțional de N procese prin pipe-uri cu nume. Procesele sunt create în lanț. N este citit de pe linia de comandă printr-o funcție din familia getopt___().
Data livrarii: 05.04.2021

Student: Alexandru Iulian Ticu
An 3 IA (reexaminare)

**/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <errno.h>

#define FIFO_NAME "lab06t01FIFO"

int main(int argc, char** argv){

	int N, opt, status, fd0, fd1, i;
	pid_t childPid;
//	char message[128] = "Salut";

	mode_t fifoPerms = S_IRUSR | S_IWUSR | S_IRGRP| S_IROTH; // rw- r-- r--

	extern int opterr, optopt;
	extern char *optarg;
	opterr = 0; // Dezactiveaza eroarea functiei getopt() (Ex: ./main: invalid option -- 'm').

	// Se verifica daca exista argumente pe linia de comanda.

	if (argc < 2){
		fprintf(stderr, "Utilizare: ./lab05t01 -n {2-9}\n");
		exit(EXIT_FAILURE);
	}

	while ((opt = getopt(argc, argv, "n:")) != -1){
		switch (opt){
			case 'n':
			// Se verifica daca N este numeric.
				for (int i=0; i<strlen(optarg); i++){
					if (!isdigit(optarg[i])){
					fprintf(stderr, "Eroare: argument optiune [2..9] valoare gresita: %s\n", optarg);
					exit(EXIT_FAILURE);
					}
				}
				fprintf(stderr, "Prelucrez optiune n, argument %s de lungime %ld\n", optarg, strlen(optarg));

				N = atoi(optarg);
				// Se verifica daca N este cuprins intre 2 si 9.
				if (N < 2 || N > 9){
					fprintf(stderr, "Eroare: argument optiune [2..9] valoare gresita: %s\n", optarg);
					exit(EXIT_FAILURE);
				}
				break;

			case '?':
				// Optiunea n trebuie sa existe pe linia de comanda.
				fprintf(stderr, "Optiuniunea citita de pe linia de comanda este necunoscuta: %c (%c)\n", opt, optopt);
				exit(EXIT_FAILURE);

			default:
				fprintf(stderr, "Optiuniunea citita de pe linia de comanda este necunoscuta\n");
				exit(EXIT_FAILURE);
		}
	}


	fprintf(stderr, "[0] Procesul MAIN cu PID=%d si PPID=%d (SHELL)\n", getpid(), getppid());

	if ((mkfifo(FIFO_NAME, fifoPerms) == -1) && (errno != EEXIST)){
		perror("mkfifo(): ");
		exit(EXIT_FAILURE);
	}

	for (i=0; i<N; i++){
		if ((childPid = fork()) != 0){
			// Parinte
			// Astept copilul sa-si termine executia.
			while (wait(&status) != childPid);

			if (i == 1){
				// Primul proces trebuie sa citeasca ultimul din FIFO.
				char message[1024] = "";
				char buff[8];


				while(read(STDIN_FILENO, buff, 8) > 0) {
					// Citesc cate 8 bytes si reconstruiesc mesajul.
					strcat(message, buff);
				}
				strcat(message, "__ m-am intors.");
				fprintf(stderr, "\n\t[%d]  PID=%d, PPID=%d Mesaj receptionat: %s\n", i, getpid(), getppid(), message);
			}

			break; // Se termina fortat bucla for pentru realizarea unui lant de procese.
		}
		else {
			// Copil

			char message[1024] = "";
			char buff[8];
			
			if (i==0){
				// Primul proces trimite mesajul "salut" atunci cand este in postura de copil.

				fd0 = open(FIFO_NAME, O_RDONLY | O_NONBLOCK);
				fd1 = open(FIFO_NAME, O_WRONLY);

				dup2(fd0, STDIN_FILENO);
				dup2(fd1, STDOUT_FILENO);
				close(fd0);
				close(fd1);

				sprintf(message, "%s%d", "Salut!", i+1);
				write(STDOUT_FILENO, message, strlen(message)+1);
				fprintf(stderr, "\n\t[%i]  PID=%d, PPID=%d Mesaj emis: Salut!1\n", i+1, getpid(), getppid());
			}
			else {
				// Celelalte procese copil fac atat read cat si write.


				while(read(STDIN_FILENO, buff, 8) > 0) {
					// Citesc cate 8 bytes si reconstruiesc mesajul.
					strcat(message, buff);
				}
				fprintf(stderr, "\n\t[%d]  PID=%d, PPID=%d Mesaj receptionat: %s\n", i+1, getpid(), getppid(), message);

				sprintf(message, "%s%d", message, i+1);
				write(STDOUT_FILENO, message, strlen(message)+1);
			}
		}
	}


	exit(EXIT_SUCCESS);
}

/**
alext@alext:~/work/PCD/t6$ gcc -o lab06t01 lab06t01.c
alext@alext:~/work/PCD/t6$ ./lab06t01
Utilizare: ./lab05t01 -n {2-9}
alext@alext:~/work/PCD/t6$ ./lab06t01 -m10
Optiuniunea citita de pe linia de comanda este necunoscuta: ? (m)
alext@alext:~/work/PCD/t6$ ./lab06t01 -n1
Prelucrez optiune n, argument 1 de lungime 1
Eroare: argument optiune [2..9] valoare gresita: 1
alext@alext:~/work/PCD/t6$ ./lab06t01 -n10
Prelucrez optiune n, argument 10 de lungime 2
Eroare: argument optiune [2..9] valoare gresita: 10
alext@alext:~/work/PCD/t6$ ./lab06t01 -na
Eroare: argument optiune [2..9] valoare gresita: a
alext@alext:~/work/PCD/t6$ ./lab06t01 -n
Optiuniunea citita de pe linia de comanda este necunoscuta: ? (n)
alext@alext:~/work/PCD/t6$ ./lab06t01 -n5
Prelucrez optiune n, argument 5 de lungime 1
[0] Procesul MAIN cu PID=9282 si PPID=4035 (SHELL)

	[1]  PID=9283, PPID=9282 Mesaj emis: Salut!1

	[2]  PID=9284, PPID=9283 Mesaj receptionat: Salut!1

	[3]  PID=9285, PPID=9284 Mesaj receptionat: Salut!12

	[4]  PID=9286, PPID=9285 Mesaj receptionat: Salut!123

	[5]  PID=9287, PPID=9286 Mesaj receptionat: Salut!1234

	[1]  PID=9283, PPID=9282 Mesaj receptionat: Salut!12345__ m-am intors.
**/