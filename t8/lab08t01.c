/**
Titlul temei:     Program  care prin mecanismul de gestiue a semnalelor realizează    comunicarea dintre un proces părinte (P1) și un proces copil (P2)    pe baza a două semnale SIGUSR2 și respectiv SIGINT. 
    Procesul copil va emite un semnal SIGUSR2 către procesul parinte.    La rândul său procesul parinte va trimite semnalul SIGINT către procesul copil.
   Tratarea semnalelor SIGUSR2 se face intr-un handler de semnal definit in procesul parinte,    iar tratarea semnalului SIGINT este lasata in seama sistemului de operare.
Data livrarii: 18.04.2021

Student: Alexandru Iulian Ticu
An 3 IA (reexaminare)


**/

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>

int SIGUSR2_handler(int signum);

int main(int argc, char** argv){

	int status;
	pid_t childPid;
	sigset_t sigMask, sigOldMask, sigMaskAll;

	if ((childPid = fork()) == 0){
		// Copil
		
		fprintf(stderr, "Copil: PID= %d, PPID= %d - emis SIGUSR2\n", getpid(), getppid());

		// sigprocmask(SIG_SETMASK, NULL, &sigOldMask); // Memorez masca initiala.

		// sigemptyset(&sigMask); // Initializez masca goala.
		// sigaddset(&sigMask, SIGINT); // Adaug SIGINT in masca.

		// sigprocmask(SIG_BLOCK, &sigMask, NULL); // SIGINT va fi blocat.
		
		kill(getppid(), SIGUSR2);

		while (1){}

	}
	else {
		// Parinte

		int status;
		struct sigaction SIGUSR2_action;

		sigprocmask(SIG_SETMASK, NULL, &sigOldMask);

		sigfillset(&sigMask);
		sigprocmask(SIG_SETMASK, &sigMask, NULL);

		SIGUSR2_action.sa_flags = 0;
		SIGUSR2_action.sa_handler = (void (*))SIGUSR2_handler;

		if (sigaction(SIGUSR2, &SIGUSR2_action, NULL) == -1){
			perror("sigaction");
			exit(1);
		}
		sigdelset(&sigMask, SIGUSR2);

		fprintf(stderr, "Parinte: PID= %d, PPID= %d PidCOPIL= %d. Astept intreruperea SIGUSR2\n", getpid(), getppid(), childPid);

		sigsuspend(&sigMask); // Astept SIGUSR2.

		fprintf(stderr, "Parinte: PID=%d. Am primit SIGUSR2 de la Copil=%d si am iesit din ciclul infinit\nParinte: emit SIGINT\n", getpid(), childPid);

		kill(childPid, SIGINT);

		// ret = pause();
		// errNO = errno;


	}


	exit(EXIT_SUCCESS);
}

int SIGUSR2_handler(int signum){

	fprintf(stderr, "Parinte: PID= %d, PPID= %d -- sunt in secventa de tratare SIGUSR2\n", getpid(), getppid());

}

/**
alext@alext:~/work/PCD/t8$ gcc -o lab08t01 lab08t01.c
alext@alext:~/work/PCD/t8$ ps
  PID TTY          TIME CMD
  652 pts/0    00:00:00 ps
 4159 pts/0    00:00:01 bash
alext@alext:~/work/PCD/t8$ ./lab08t01 
Parinte: PID= 655, PPID= 4159 PidCOPIL= 656. Astept intreruperea SIGUSR2
Copil: PID= 656, PPID= 655 - emis SIGUSR2
Parinte: PID= 655, PPID= 4159 -- sunt in secventa de tratare SIGUSR2
Parinte: PID=655. Am primit SIGUSR2 de la Copil=656 si am iesit din ciclul infinit
Parinte: emit SIGINT
**/