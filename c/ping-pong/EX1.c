#define _POSIX_C_SOURCE 200899L	/*SA_RESTART*/
#include <stdio.h> 				/* printf */
#include <signal.h> 			/* sigaction */
#include <unistd.h> 			/* pid_t, fork */

#define PARENT 1
#define CHILD 0

void parent_action()
{
	write(1, "ping 1\n", 7);
}

void child_action()
{
	write(1, "pong		2\n", 8);
}

int main()
{
	pid_t pid = 0;

	struct sigaction child_signal_handler = {0};
	struct sigaction parent_signal_handler = {0};

    child_signal_handler.sa_handler = child_action;
    sigemptyset(&child_signal_handler.sa_mask);
    child_signal_handler.sa_flags = SA_RESTART;

	parent_signal_handler.sa_handler = parent_action;
    sigemptyset(&parent_signal_handler.sa_mask);
    parent_signal_handler.sa_flags = SA_RESTART;

	pid = fork();
	if (CHILD == pid)
	{
		printf("in child process, child pid %d\n", pid);
		sigaction(SIGUSR1, &child_signal_handler, NULL);
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
			kill(getppid(), SIGUSR2);

		}
		else
		{
			kill(pid, SIGUSR1);
		}
		sleep(1);
	}

	return 0;
}
