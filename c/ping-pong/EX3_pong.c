#define _POSIX_C_SOURCE 200899L	/*SA_RESTART*/
#include <stdio.h> 				/* printf */
#include <signal.h> 			/* sigaction */
#include <unistd.h> 			/* pid_t*/

pid_t proc1_pid = 0;

void proc2_action(int sig, siginfo_t *info, void *ucontext)
{
	write(1, "pong		2\n", 8);
	proc1_pid = info->si_pid;
}

int main()
{
	struct sigaction proc2_signal_handler = {0};

    proc2_signal_handler.sa_sigaction = proc2_action;
    sigemptyset(&proc2_signal_handler.sa_mask);
    proc2_signal_handler.sa_flags = SA_SIGINFO;

	sigaction(SIGUSR1, &proc2_signal_handler, NULL);
	
	sleep(10); /*In order to hold proc2 back from sending SIGUSR2 too early*/

	while(1)
	{
		kill(proc1_pid, SIGUSR2);
		sleep(1);
	}

	return 0;
}
