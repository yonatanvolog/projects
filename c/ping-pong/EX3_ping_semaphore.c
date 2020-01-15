/*
Steps for proper compilation:
gcc -o EX3_pong_semaphore EX3_pong_semaphore.c -lpthread -lrt
gcc -o EX3_ping_semaphore EX3_ping_semaphore.c -lpthread -lrt

./EX3_pong_semaphore &

get _pong_pid_ using htop

./EX3_ping_semaphore _pong_pid_
*/

#define _POSIX_C_SOURCE 200899L	/*SA_RESTART */
#include <stdio.h> 				/* printf */
#include <signal.h> 			/* sigaction */
#include <unistd.h> 			/* pid_t*/
#include <stdlib.h>				/*atoi*/

#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>

void proc1_action()
{
	write(1, "ping 1\n", 7);
}

int main(int argc,char *argv[])
{
	pid_t proc2_pid = atoi(argv[1]);
	
	struct sigaction proc1_signal_handler = {0};

    sem_t *sem = sem_open("/semaphore", 0);

	printf("proc2_pid: %d\n",proc2_pid);

	proc1_signal_handler.sa_handler = proc1_action;
    sigemptyset(&proc1_signal_handler.sa_mask);
    proc1_signal_handler.sa_flags = SA_RESTART;
	sigaction(SIGUSR2, &proc1_signal_handler, NULL);

	sem_post(sem);
	
	while(1)
	{
		kill(proc2_pid, SIGUSR1);
		sleep(1);
	}

	return 0;
}
