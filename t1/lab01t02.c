/**
Titlul temei: Analizarea argumentelor introduse pe linia de comanda prin apel functie getopt_long() si getopt_long_only()
Data livrarii: 07.03.2021

Student: Alexandru Iulian Ticu
An 3 IA (recontractare)


<Functia getopt_long_only se poate interschimba cu getopt_long. Singura diferenta dintre acestea 2 este ca getopt_long_only poate considera '-' si '--' ca fiind optiune lunga (daca exista), pe cand getopt_long considera doar '--' ca fiind optiune lunga>

**/

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <pthread.h>
#include <unistd.h>

int threadNum;

void* run_th(void* arg){

	// Type cast din void* in int*.
	int* args = (int *)(arg);
	int timer = args[0];
	int sayHello = args[1];
	if (sayHello) {
		printf("Firul de executie %d te saluta!\n", threadNum++);
	}
	sleep(timer);
}


int main(int argc, char **argv){

	int c, option_index = 0;
	static int sayHello = 0, skip = 0;
	int timer = 2;
	int numThreads;

	static struct option long_options[] = {
		// Porneste --threads sau -t fire de executie
		{"threads", required_argument, 0, 't'},
		// Fiecare fir de executie dureaza aproximativ --time (daca este setat)
		{"timer", required_argument, 0, 1},
		// Firele de executie printeaza "Salut!". Este folosit pentru a evidentia folosirea flag-ului (&sayHello) din structura.
		{"say-hello", no_argument, &sayHello, 1},
		// Se foloseste skip ca sa nu se mai porneasca firele de executie.
		{"skip", no_argument, &skip, 1},
		{0, 0, 0, 0}
	};

	while((c = getopt_long(argc, argv, "at:", long_options, &option_index)) != -1) {

		switch(c) {
		case 0:
			printf("Se seteaza %s\n", long_options[option_index].name);
			break;
		case 1:
			if ((timer = atoi(optarg)) < 1) break;
			printf("Timer pentru firele de executie: %d\n", timer);
			break;

		case 't':
			if ((numThreads = atoi(optarg)) == 0) break;
			break;

		case 'a':
			printf("Optiunea -a nu face nimic special\n");
			break;

		case '?':
			break;

		default:
			break;	
		}
	}

	if(!skip){

		int args[] = {timer, sayHello};

		printf("Start %d fire de executie...\n", numThreads);
		pthread_t threads[numThreads];
		for (int i=0; i<numThreads; i++){
			pthread_create(&threads[i], NULL, run_th, args);
		}

		pthread_exit(NULL);
	}


	return 0;
}

/**
$ gcc lab01t01.c -o main -lpthread

$ ./main -t 5 --timer 2 --say-hello -a
Timer pentru firele de executie: 2
Se seteaza say-hello
Optiunea -a nu face nimic special
Start 5 fire de executie...
Firul de executie 0 te saluta!
Firul de executie 1 te saluta!
Firul de executie 2 te saluta!
Firul de executie 3 te saluta!
Firul de executie 4 te saluta!

$  ./main -t 5 --timer 2 --say-hello -a --skip
Timer pentru firele de executie: 2
Se seteaza say-hello
Optiunea -a nu face nimic special
Se seteaza skip 
**/
