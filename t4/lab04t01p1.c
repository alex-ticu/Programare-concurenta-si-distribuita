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
	pid_t pid, ppid;

	struct msgbuf{
		long mtype;
		char mtext[80];
	};

	struct msgbuf msg;

	pid = getpid();
	ppid = getppid();

	// Afisez pe stderr PID si PPID;

	fprintf(stderr, "P1: PID= %d\nP1: PPID= %d\n", pid, ppid);

	if ((msgKey = ftok(MSG_KEY, MSG_ID)) == -1) {
		perror("Apelul functiei ftok a esuat: ");
		exit(EXIT_FAILURE);
	}

	printf("P1: MSG key: 0x%08x\n", msgKey);

	// Creez o coada de mesaje folosing cheia creata de ftok();

	if ((msgID = msgget(msgKey, IPC_CREAT | 0666)) == -1){
		perror("Apelul functiei msgget a esuat: ");
		exit(EXIT_FAILURE);
	}

	// Creez mesajul si il trimit;

	msg.mtype = 1;
	sprintf(msg.mtext, "PIDp1= %d; PPIDp1=%d", pid, ppid);

	if (msgsnd(msgID, &msg, sizeof(msg.mtext), IPC_NOWAIT) == -1){
		perror("Apelul functiei msgsnd a esuait: ");
		exit(EXIT_FAILURE);
	}

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
