#define _POSIX_C_SOURCE 200899L	/*SA_RESTART*/
#include <stdio.h> 				/* printf */
#include <signal.h> 			/* sigaction */
#include <unistd.h> 			/* pid_t, fork */

#define CHILD 0
#define PARENT 1

void child_action()
{
	write(1, "pong		2\n", 8);
}

int main()
{
	struct sigaction child_signal_handler = {0};

    child_signal_handler.sa_handler = child_action;
    sigemptyset(&child_signal_handler.sa_mask);
    child_signal_handler.sa_flags = SA_RESTART;

	sigaction(SIGUSR1, &child_signal_handler, NULL);
	
	sleep(1); /*In order to hold child back from sending SIGUSR2 too early*/

	while(1)
	{
		kill(getppid(), SIGUSR2);
		sleep(1);
	}

	return 0;
}
