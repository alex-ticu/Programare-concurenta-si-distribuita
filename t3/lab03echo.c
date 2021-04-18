/**
Titlul temei: -
Data livrarii: 14.03.2021

Student: Alexandru Iulian Ticu
An 3 IA (recontractare)

<Program folosit pentru a demonstra functionalitatea celolrlalte programe trimise ca tema.>

**/

#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]){

	printf("\tPROCES ECHO:: proces ID=%ld, parinte ID= %ld\n", (long)getpid(), (long)getppid());
	for (int j = 0; j < argc; j++)
		printf("\tPROCES ECHO:: argv[%d]: %s\n", j, argv[j]);


	// Operatorul ? este folosit astfel incat sa nu se permita printarea lui null daca variabila nu exista (Similar cu echo ${CUSTOM_VAR}).
	printf("\tPROCES ECHO:: PATH= %s\n\tPROCES ECHO:: CUSTOM_VAR= %s\n", getenv("PATH") == NULL ? "" : getenv("PATH"), getenv("CUSTOM_VAR") == NULL ? "" : getenv("CUSTOM_VAR"));
	exit(EXIT_SUCCESS);
}

/**

gcc lab03echo.c -o echo_

**/