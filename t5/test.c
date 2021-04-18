/**
Lab. 05 Tema 1
Titlul temei: 1. Realizarea unui token-ring unidirecțional de N procese prin pipe-uri anonime.
Procesele sunt create în lanț. N este citit de pe linia de comandă printr-o funcție din familia getopt___().
data livrarii: 28.03.2021
Student: Costi Flavia-Emanuela
An 3 IA
----------

Programul creaza n procese prin pepe-uri anonime. Initial citim procesul parinte si procesul copil, iar la finalul
strabaterii celor n procese vom reveni la procesul start. Utilizam functiile read si write pentru scrierea si
pentru citirea unui mesaj dat de catre noi (in acest caz <<Salut>>).
Redirectarea se face cu ajutorul lui dup2 de la procesul int, la noul int.
Afisam pentru fiecare i proces din sirul de n procesul parinte si pe sine.
Bineinteles ca aceste lucruri se realizeaza daca e satisfacuta conditia din linia de comanda a apelului programului.
Cu ajutorul lui getopt() vom valida sau vom arunca o eroare (daca este cazul), argumentele trimise pe linia de comanda.

**/
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include<fcntl.h>
#include <errno.h>
#include <string.h>
#include <wait.h>

#define OPT_LIST "n:"

int main(int argc, char *argv[])
{
    int nr=-1;
    int c;
    int optiune;
    opterr=0 ;       // variabila globala opterr=0, atunci eroarea e tratata numai de "?"
    int n=-1;  		// n= numar procese din lant din getopt()
    while((c = getopt(argc, argv, "n:")) != -1)	// control al linei de comanda
   								// n: = optiune -n urmata de un argument citit in  optarg
	 //utilizam getopt pentru a citi si valida argumentele de pe linia de comanda
	{						
      switch(c)
        {
           case 'n' :	// primeste un sir de caractere dupa optiunea -n 
             //se executa testarea lui N
             printf("\tPrelucrez optiune %c,  argument %s de lungime %ld\n", c, optarg, strlen (optarg));
 
			    /* secventa de verificare argument N dat pe linia de comanda */
			    if (optarg[0] > '9' || optarg[0] <  '2' || strlen(optarg) >= 2)
			    {  // eroare valoare optiune 
        		   fprintf(stderr, "Eroare: argument optiune [2...9]  valoare gresita: %s\n",  optarg); 
        		   exit(EXIT_FAILURE);
			    }
			    nr= atoi(optarg);
             break;
          case  '?' : // daca  opterr = 0 atunci sistemul nu mai zice eroare
           fprintf(stderr, "Optiuniunea citita de pe linia de comanda este necunoscuta: %c \n", c);
            exit(EXIT_FAILURE);
        }
     }
     if (nr == -1) 
     {
	  fprintf(stderr, "Utilizare: %s fara argumente\n",  argv[0]);
      exit(EXIT_FAILURE); 
     }	   
         
         //continuam doar in cazul in care am scris un argument valid pe linia de comanda
        int fd[2];
        int p;
        int status;
        int pid,i=0;
        char buf[20];
        size_t bytes_read;
        char message[100]="1234567890";   // pentru a fi sigur ca treaba merge, initializam cu o valoare oarecare
		  fprintf(stderr, "\n [0] Procesul MAIN cu PID=%ld si PPID=%ld (SHELL)\n", 
                 (long) getpid(), (long)getppid());
        //creare cele n procese si intoarecerea la primul proces
        for(i=1;i<=nr;i++){
             //cream copilul procesului
 	         if(p=fork())
	         {// daca procesul este parinte astept terminarea copiilor
 			   while (wait(&status) != p){;};	 /* empty: se asteapta terminarea copilului */
            // proces MAIN  copil 1
            if ((i==2))    // sunt in secventa main a primului copil ???
            { 
             // citesc si afisez ultim mesaj
 	          read(STDIN_FILENO, &message,sizeof(message)); // asteapta mesaj de la ultimul proces in ring
             fprintf(stderr, 
             "\n\t[%d]  PID=%ld, PPID=%ld Mesaj receptionat: %s\n", 
               i-1, (long) getpid(), (long)getppid(), strcat(message, "___m-am intors")); 
            }
 	          break; // in parinte terminare fortata bucla for
            }
            //daca procesul este copil
            else{
 	         switch (i)
	        {
             case 1:
	          //  primul copil atunci numai write  in pipe pentru startare token
	              // apelam pipe pentru conectarea lui stdInput la stdOutput
             if (pipe(fd) == -1) { perror("pipe");  exit(EXIT_FAILURE);};
        	    dup2(fd[0],STDIN_FILENO);
             dup2(fd[1],STDOUT_FILENO);
             close(fd[0]);
             close(fd[1]);
	          sprintf(message, "%s%d","Salut...", i);
             write(STDOUT_FILENO, message,strlen(message));    // primul copil scrie in pipe mesajul
             fprintf(stderr, "\n\t[%i]  PID=%ld, PPID=%ld Mesaj emis: %s\n", 
                   i, (long) getpid(), (long)getppid(), message);  // identificare proces             	
              break;	
             default:  // daca nu e primul copil atunci  read si write
                read(STDIN_FILENO, &message,sizeof(message));   // urmatorul copil citeste din pipe mesajul	
                fprintf(stderr, 
                   "\n\t[%i]  PID=%ld, PPID=%ld Mesaj receptionat: %s\n", 
                   i, (long) getpid(), (long)getppid(), message);	
               //write
               sprintf(message, "%s%d",message, i);
               write(STDOUT_FILENO, &message,strlen(message));    //  trimite mai departe urmatorului copil	 
               break;
              }

             }  // end copil
         } // end for

       exit(0);
   }  // end proces main main

/**
$ gcc -o tab05t01 tab05t01.c

$ ./2
Utilizare: ./2 fara argumente

$ ./2 -m10
Optiuniunea citita de pe linia de comanda este necunoscuta: ? 

$ ./2 -n1
	Prelucrez optiune n,  argument 1 de lungime 1
Eroare: argument optiune [2...9]  valoare gresita: 1

$ ./2 -n10
	Prelucrez optiune n,  argument 10 de lungime 2
Eroare: argument optiune [2...9]  valoare gresita: 10

$ ./2 -na
	Prelucrez optiune n,  argument a de lungime 1
Eroare: argument optiune [2...9]  valoare gresita: a

$ ./2 -n.
	Prelucrez optiune n,  argument . de lungime 1
Eroare: argument optiune [2...9]  valoare gresita: .

$ ./2 -n5
	Prelucrez optiune n,  argument 5 de lungime 1

 [0] Procesul MAIN cu PID=3524 si PPID=3063 (SHELL)

	[1]  PID=3525, PPID=3524 Mesaj emis: Salut...1

	[2]  PID=3526, PPID=3525 Mesaj receptionat: Salut...1

	[3]  PID=3527, PPID=3526 Mesaj receptionat: Salut...12

	[4]  PID=3528, PPID=3527 Mesaj receptionat: Salut...123

	[5]  PID=3529, PPID=3528 Mesaj receptionat: Salut...1234

	[1]  PID=3525, PPID=3524 Mesaj receptionat: Salut...12345___m-am intors
$ 
**/