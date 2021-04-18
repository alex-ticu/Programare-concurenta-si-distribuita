/**
Titlul temei: Să se scrie un program care prin mecanismul de fork crează o structură ierarhizată de procese de 2 nivele fiecare nivel avand N, respectiv M procese în pieptene
Data livrarii: 07.03.2021

Student: Alexandru Iulian Ticu
An 3 IA (recontractare)

**/

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


int main(int argc, char** argv){

	int c, N = -1, M = -1;

	if(argc < 3){
		fprintf(stderr, "Argumentele N si M sunt obligatorii!\n");
		exit(1);
	}

	while((c = getopt(argc, argv, "N:M:") ) != -1){
		switch(c){
		case 'N':
			if((N = atoi(optarg)) < 1) {
				fprintf(stderr, "N trebuie sa fie mai mare ca 0!\n");
				exit(1);
			}
			break;

		case 'M':
			if((M = atoi(optarg)) < 1) {
				fprintf(stderr, "M trebuie sa fie mai mare ca 0!\n");
				exit(1);
			}
			break;

		default:
			fprintf(stderr, "Usage: %s -N int -M int\n", argv[0]);
			exit(EXIT_FAILURE);
			break;
		}
	}

	if (N < 1 || M < 1) {
		fprintf(stderr, "N si M trebuie sa fie mai mare ca 0!\n");
		exit(1);
	}

	pid_t childpid;
	int status;

	fprintf(stderr, "Acesta este procesul initial PARINTE %ld \n", (long)getpid());
	fflush(stderr);

	// Creare piptene nivel 1 (N).
	for(int i=0; i<N; i++){
		if((childpid = fork()) == 0){
			fprintf(stderr, "Acesta este procesul COPIL[%d] (nivel 1) %ld cu parintele %ld\n", i, (long)getpid(), (long)getppid());
			fflush(stderr);

			// Creare pieptene nivel 2 (M).
			for(int j=0; j<M; j++){
				if((childpid = fork()) == 0) {
					fprintf(stderr, "\tAcesta este procesul COPIL[%d] (nivel 2) %ld cu parintele %ld\n", j, (long)getpid(), (long)getppid());
					fflush(stderr);
					break;
				}else {
					while(wait(&status) != childpid);
					
				} 
			}

			break;
		} else{
			while(wait(&status) != childpid);
		}
	}
	

	return 0;
}
/**
$ gcc lab02t01.c -o main

./main -N 4 -M 4
Acesta este procesul initial PARINTE 25403
Acesta este procesul COPIL[0] (nivel 1) 25404 cu parintele 25403
	Acesta este procesul COPIL[0] (nivel 2) 25405 cu parintele 25404
	Acesta este procesul COPIL[1] (nivel 2) 25406 cu parintele 25404
	Acesta este procesul COPIL[2] (nivel 2) 25407 cu parintele 25404
	Acesta este procesul COPIL[3] (nivel 2) 25408 cu parintele 25404
Acesta este procesul COPIL[1] (nivel 1) 25409 cu parintele 25403
	Acesta este procesul COPIL[0] (nivel 2) 25410 cu parintele 25409
	Acesta este procesul COPIL[1] (nivel 2) 25411 cu parintele 25409
	Acesta este procesul COPIL[2] (nivel 2) 25412 cu parintele 25409
	Acesta este procesul COPIL[3] (nivel 2) 25413 cu parintele 25409
Acesta este procesul COPIL[2] (nivel 1) 25414 cu parintele 25403
	Acesta este procesul COPIL[0] (nivel 2) 25415 cu parintele 25414
	Acesta este procesul COPIL[1] (nivel 2) 25416 cu parintele 25414
	Acesta este procesul COPIL[2] (nivel 2) 25417 cu parintele 25414
	Acesta este procesul COPIL[3] (nivel 2) 25418 cu parintele 25414
Acesta este procesul COPIL[3] (nivel 1) 25419 cu parintele 25403
	Acesta este procesul COPIL[0] (nivel 2) 25420 cu parintele 25419
	Acesta este procesul COPIL[1] (nivel 2) 25421 cu parintele 25419
	Acesta este procesul COPIL[2] (nivel 2) 25422 cu parintele 25419
	Acesta este procesul COPIL[3] (nivel 2) 25423 cu parintele 25419

./main -N 1 -M 2
Acesta este procesul initial PARINTE 25479
Acesta este procesul COPIL[0] (nivel 1) 25480 cu parintele 25479
	Acesta este procesul COPIL[0] (nivel 2) 25481 cu parintele 25480
	Acesta este procesul COPIL[1] (nivel 2) 25482 cu parintele 25480

./main -N 2 -M 3
Acesta este procesul initial PARINTE 25485 
Acesta este procesul COPIL[0] (nivel 1) 25486 cu parintele 25485
	Acesta este procesul COPIL[0] (nivel 2) 25487 cu parintele 25486
	Acesta este procesul COPIL[1] (nivel 2) 25488 cu parintele 25486
	Acesta este procesul COPIL[2] (nivel 2) 25489 cu parintele 25486
Acesta este procesul COPIL[1] (nivel 1) 25490 cu parintele 25485
	Acesta este procesul COPIL[0] (nivel 2) 25491 cu parintele 25490
	Acesta este procesul COPIL[1] (nivel 2) 25492 cu parintele 25490
	Acesta este procesul COPIL[2] (nivel 2) 25493 cu parintele 25490
**/
