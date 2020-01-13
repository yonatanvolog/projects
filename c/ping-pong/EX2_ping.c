/*
Steps for porper compilation:
gcc -o EX2_pong EX2_pong.c
gcc -o EX2_ping EX2_ping.c
./EX2_ping
*/

#define _POSIX_C_SOURCE 200899L	/*SA_RESTART */
#include <stdio.h> 				/* printf */
#include <signal.h> 			/* sigaction */
#include <unistd.h> 			/* pid_t, fork */

#define CHILD 0
#define PARENT 1

void parent_action()
{
	write(1, "ping 1\n", 7);
}

int main()
{
	pid_t pid = 0;

	struct sigaction parent_signal_handler = {0};

	parent_signal_handler.sa_handler = parent_action;
    sigemptyset(&parent_signal_handler.sa_mask);
    parent_signal_handler.sa_flags = SA_RESTART;

	pid = fork();
	if (CHILD == pid)
	{
		char *args[]={"./EX2_pong",NULL};
        execvp(args[0],args); 
	}
	else
	{
		printf("in parent process, child pid %d\n", pid);
		sigaction(SIGUSR2, &parent_signal_handler, NULL);
	}
	
	sleep(1); /*In order to hold child back from sending SIGUSR2 too early*/

	while(1)
	{
		if (CHILD == pid)
		{
			printf("error, child pid is: %d\n", pid);

			return 1;
		}
		else
		{
			kill(pid, SIGUSR1);
		}
		sleep(1);
	}

	return 0;
}
