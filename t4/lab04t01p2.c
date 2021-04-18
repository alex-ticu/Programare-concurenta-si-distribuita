/**
Titlul temei: Utilizarea de tehnici IPC de tip "coadă de mesaje". Crearea a două procese P1 și P2. Procesele NU sunt în relație de părinte-copil. Procesul 1 crează o coadă și trimite un mesaj în coadă./ Procesul 2 Citește mesajul din coadă.
Data livrarii: 22.03.2021

Student: Alexandru Iulian Ticu
An 3 IA (recontractare)

**/

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>

#define MSG_KEY "./"
#define MSG_ID 42

int main(int argc, char** argv){

	key_t msgKey;
	int msgID;
	struct msqid_ds buf;

	struct msgbuf{
		long mtype;
		char mtext[80];
	};
	struct msgbuf msg;

	// Afiseaza pe stderr PID si PPID;

	fprintf(stderr, "P2: PID= %d\nP2: PPID= %d\n", getpid(), getppid());

	if ((msgKey = ftok(MSG_KEY, MSG_ID)) == -1) {
		perror("Apelul functiei ftok a esuat: ");
		exit(EXIT_FAILURE);
	}

	if ((msgID = msgget(msgKey, 0)) == -1){
		perror("Apelul functiei msgget a esuat: ");
		exit(EXIT_FAILURE);
	}

	if (msgctl(msgID, IPC_STAT, &buf) == -1){
		perror("Apelul functiei msgctl a esuat: ");
		exit(EXIT_FAILURE);
	}

	printf("Citesc %ld mesaje din coada...\n", buf.msg_qnum);

	// Citeste ciclic mesajele din coada pana la golirea cozii;

	while (buf.msg_qnum > 0){

		if (msgrcv(msgID, &msg, sizeof(msg.mtext), 1, MSG_NOERROR | IPC_NOWAIT) == -1){
			if (errno != ENOMSG) {
				perror("Apelul functiei msgrcv a esuat: ");
				exit(EXIT_FAILURE);
			}
			// Daca mesajul ar fi gol;
			printf("Mesajul este gol...");
		} else{
			// Afiseaza mesajul;
			printf("%s\n", msg.mtext);
		}

		// Actualizez structura de date;
		if (msgctl(msgID, IPC_STAT, &buf) == -1){
			perror("Apelul functiei msgctl a esuat: ");
			exit(EXIT_FAILURE);
		}
	}

	// Sterg coada de mesaje;

	msgctl(msgID, IPC_RMID, NULL);

	exit(EXIT_SUCCESS);
}

/**
alext@alext:~/work/PCD/t4$ gcc lab04t01p1.c -o P1
alext@alext:~/work/PCD/t4$ ./P1 
P1: PID= 19861
P1: PPID= 14459
P1: MSG key: 0x2a021170
alext@alext:~/work/PCD/t4$ ipcs -q

------ Message Queues --------
key        msqid      owner      perms      used-bytes   messages    
0x2a021170 3          alext      666        80           1           

alext@alext:~/work/PCD/t4$ ./P1 
P1: PID= 19863
P1: PPID= 14459
P1: MSG key: 0x2a021170
alext@alext:~/work/PCD/t4$ ipcs -q

------ Message Queues --------
key        msqid      owner      perms      used-bytes   messages    
0x2a021170 3          alext      666        160          2           

alext@alext:~/work/PCD/t4$ ./P1 
P1: PID= 19866
P1: PPID= 14459
P1: MSG key: 0x2a021170
alext@alext:~/work/PCD/t4$ ipcs -q

------ Message Queues --------
key        msqid      owner      perms      used-bytes   messages    
0x2a021170 3          alext      666        240          3           

alext@alext:~/work/PCD/t4$ gcc lab04t01p2.c -o P2
alext@alext:~/work/PCD/t4$ ./P2 
P2: PID= 19878
P2: PPID= 14459
Citesc 3 mesaje din coada...
PIDp1= 19861; PPIDp1=14459
PIDp1= 19863; PPIDp1=14459
PIDp1= 19866; PPIDp1=14459
alext@alext:~/work/PCD/t4$ ipcs -q

------ Message Queues --------
key        msqid      owner      perms      used-bytes   messages
**/