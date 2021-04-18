/**
Titlul temei: Realizarea unui token-ring unidirecțional de N procese prin pipe-uri anonime. Procesele sunt create în lanț. N este citit de pe linia de comandă printr-o funcție din familia getopt___().
Data livrarii: 28.03.2021

Student: Alexandru Iulian Ticu
An 3 IA (recontractare)

**/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/wait.h>


int main(int argc, char** argv){

	int N, opt, status;
	char message[] = "Salut";

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

	fprintf(stderr, "Se vor genera %d procese in lant.\n", N);

	pid_t childPid;

	if ((childPid = fork()) != 0){
		fprintf(stderr, "[0] Procesul MAIN cu PID=%d si PPID=%d (SHELL)\n", getpid(), getppid());
		wait(NULL);
 	} 
 	else {
 		int i, fd[2];
 		char buff[32];
 		for (i=1; i<N; i++){
 //			fprintf(stderr, "~~~[%d] (%d) (%d)\n", i, getpid(), getppid());
 			pipe(fd[i]);

 			if ((childPid = fork()) != 0){
 //				fprintf(stderr, "~~~[%d] (%d) (%d)\n", i, getpid(), getppid());
 				// Parinte
 				if (i == 1){
 					

 					dup2(fd[i][1], STDOUT_FILENO);

 					
 					close(fd[0][0]);
 					close(fd[i][0]);
 					

 					write(STDOUT_FILENO, message, strlen(message));
 					close(fd[i][1]);
 					close(fd[0][1]);
 					fprintf(stderr, "[%d] PID=%d, PPID=%d Mesaj emis: %s\n", i, getpid(), getppid(), message);
 				}
 				else {
 					dup2(fd[i][1], STDOUT_FILENO);

 					close(fd[i][0]);
 					

 					write(STDOUT_FILENO, buff, strlen(buff));
 					close(fd[i][1]);
 				}
				while (wait(&status) != childPid){;};
				break;
 			}
 			else {
 				if (i+1 == N) {
// 					fprintf(stderr, "~~~[%d] (%d) (%d)\n", i, getpid(), getppid());
 					fprintf(stderr, "[%d] PID=%d, PPID=%d Mesaj receptionat: ", i+1, getpid(), getppid());
 					dup2(fd[i][0], STDIN_FILENO);

 					close(fd[i][1]);
 					

 					while (read(STDIN_FILENO, buff, 32) > 0) {
 						fprintf(stderr, "%s", buff);						
 					}
 					fprintf(stderr, "\n");
 					close(fd[i][0]);

 					dup2(fd[0][1], STDOUT_FILENO);

 					close(fd[0][0]);
 					

	  				write(STDOUT_FILENO, buff, strlen(buff));
	  				close(fd[0][1]);
 				}
 				else {
 					fprintf(stderr, "[%d] PID=%d, PPID=%d Mesaj receptionat: ", i+1, getpid(), getppid());
					dup2(fd[i][0], STDIN_FILENO);

					close(fd[i][1]);
					

					while (read(STDIN_FILENO, buff, 32)) { // Al 2-lea proces se blocheaza la acest read. Nu am inchis toate pipe-urile?
						fprintf(stderr, "%s", buff);
					}
//					fprintf(stderr, "[%d] PID=%d, PPID=%d Mesaj", i, getpid(), getppid());
					fprintf(stderr, "\n");
					close(fd[i][0]);
 				}
 			}
 			if (i == 1){
// 				waitpid(childPid, &status, 0);
 				fprintf(stderr, "[%d] PID=%d, PPID=%d Mesaj receptionat: ", i, getpid(), getppid());
 				dup2(fd[i][0], STDIN_FILENO);

 				close(fd[0][1]);
 				

 				while (read(STDIN_FILENO, buff, 32) > 0) {
 						fprintf(stderr, "%s", buff);						
 					}
 				fprintf(stderr, "__GATA\n");
 				close(fd[0][0]);
 			}
 		}
 	}
 	 	exit(EXIT_SUCCESS);
}

/**
alext@alext:~/work/PCD/t5$ gcc -o lab05t01 ./lab05t01.c 
alext@alext:~/work/PCD/t5$  ./lab05t01
Utilizare: ./lab05t01 -n {2-9}
alext@alext:~/work/PCD/t5$ ./lab05t01 -m10
Optiuniunea citita de pe linia de comanda este necunoscuta: ? (m)
alext@alext:~/work/PCD/t5$ ./lab05t01 -n1
Prelucrez optiune n, argument 1 de lungime 1
Eroare: argument optiune [2..9] valoare gresita: 1
alext@alext:~/work/PCD/t5$ ./lab05t01 -n10
Prelucrez optiune n, argument 10 de lungime 2
Eroare: argument optiune [2..9] valoare gresita: 10
alext@alext:~/work/PCD/t5$ ./lab05t01 -na
Eroare: argument optiune [2..9] valoare gresita: a
alext@alext:~/work/PCD/t5$  ./lab05t01 -n.
Eroare: argument optiune [2..9] valoare gresita: .
alext@alext:~/work/PCD/t5$ ./lab05t01 -n5
Prelucrez optiune n, argument 5 de lungime 1
Se vor genera 5 procese in lant.
[0] Procesul MAIN cu PID=918 si PPID=24412 (SHELL)
[1] PID=919, PPID=918 Mesaj emis: Salut
[2] PID=920, PPID=919 Mesaj receptionat: Salut
[1] PID=920, PPID=3554 Mesaj receptionat: __GATA
[3] PID=921, PPID=920 Mesaj receptionat: Salut
[4] PID=922, PPID=921 Mesaj receptionat: Salut
[5] PID=923, PPID=922 Mesaj receptionat: Salut
Salut
**/