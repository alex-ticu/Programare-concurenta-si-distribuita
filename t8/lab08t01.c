/**
Titlul temei: Să se scrie un program, care conform figurii atașate, prin mecanismul de gestiue a semnalelor realizează comunicarea dintre un proces părinte (P1) și un proces copil (P2) pe baza a două semnale SIGUSR2 și respectiv SIGINT. Procesul copil va emite un semnal SIGUSR2 către procesul părinte. La rândul său procesul părinte va trimite semnalul SIGINT către procesul copil. Tratarea semnalului SIGUSR2 se face într-un handler de semnal definit în procesul părinte, iar tratarea semnalului SIGINT este lăsată în seama sistemului de operare.
Data livrarii: 11.04.2021

Student: Alexandru Iulian Ticu
An 3 IA (reexaminare)

--
Acest program foloseste functia pause() pentru a astepta emiterea unui semnal.
--

**/

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

int SIGUSR2_handler(int signum);

int main(int argc, char** argv){


	exit(EXIT_SUCCESS);
}

int SIGUSR2_handler(int signum){

	signal(SIGUSR2, (void (*))SIGUSR2_handler);
	fprintf(stderr, "Parinte: PID=%d, PPID=%d -- sunt in secventa de tratare SIGUSR2 (%d).\n", getpid(), getppid(), signum);

}


/**
alext@alext:~/work/PCD/t7$ gcc lab07t01.c -o lab07t01
alext@alext:~/work/PCD/t7$ ./lab07t01 
Parinte: PID= 6542, PPID= 2932 PidCOPIL= 6543.
Parinte: PID= 6542. Urmeaza un apel pause() si astept SIGUSR2 de la copil.
Copil: PID= 6543, PPID= 6542. Emit SIGUSR2 (= 12) catre parinte.
Parinte: PID=6542, PPID=2932 -- sunt in secventa de tratare SIGUSR2 (12).
Parinte: PID= 6542. PPID= 2932 -- SIGUSR2 livrat de copil PID= 6543 m-a scos din pause() . Am iesit cu errno=4.
Parinte: PID= 6542, PPID= 2932 am emis SIGINT (ctrl-c) catre PidCOPIL= 6543.
**/