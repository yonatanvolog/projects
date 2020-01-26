#include "wd_utils.h"

#define OTHER_PROCESS (0) /*redefined at each translation unit*/
#define WD_PROC (1)
#define APP_PROC (2)

int IsOtherProcessAlive(void *wrapper)
{
	int interval = 0;
	int dead_time = 0;
	int max_death_time_allowed = 0;
	scheduler_t *sched = NULL;

	interval  = ((wrapper_t *)wrapper)->interval;
	dead_time = ((wrapper_t *)wrapper)->dead_time;
	sched     = ((wrapper_t *)wrapper)->sched;

	max_death_time_allowed = interval * dead_time;

	printf("no_response_from_other_process_counter: %d of %d\n", (*(((wrapper_t *)wrapper)->no_response_from_other_process_counter)), max_death_time_allowed);

	if(max_death_time_allowed <= (*(((wrapper_t *)wrapper)->no_response_from_other_process_counter)))
	{
        SchedStop(sched);
	}
	++(*(((wrapper_t *)wrapper)->no_response_from_other_process_counter));

	return 1;
}

int SendSignal(void *wrapper)
{
	pid_t other_process_pid = *(((wrapper_t *)wrapper)->other_process_pid);
	
    printf("Sending Signal to process %d\n", other_process_pid);

  	kill(other_process_pid, SIGUSR1);

	return 1;
}

int CheckDNR(void *wrapper)
{
	scheduler_t *sched = ((wrapper_t *)wrapper)->sched;

	if (1 == *(((wrapper_t *)wrapper)->dnr_flag))
	{
		SchedStop(sched);
	}
	return 1;
}

void ReviveOtherProcess(void *wrapper)
{
	pid_t pid = 0;
	char *args[30]= {NULL};
	int process_to_revive = ((wrapper_t *)wrapper)->process_to_revive;

	if(WD_PROC == process_to_revive)
	{
		args[0]= "./wd.out";
		args[1]= NULL;
	}
	else
	{
		args[0]= "./a.out";
		args[1]= NULL;
	}

	pid = fork();
	if (0 == pid)
	{
		execvp(args[0],args);
	}
	else
	{
		*(((wrapper_t *)wrapper)->other_process_pid) = pid;
	}
}

int AddTasksToSched(scheduler_t *sched, task_t task1, task_t task2, task_t task3, time_t interval, void *wrapper)
{
	ilrd_uid_t sched_add_status = {0};

	sched_add_status = SchedAdd(sched, (time_t)interval, task1, wrapper);
	if (1 == UIDIsBad(sched_add_status))
	{
		SchedDestroy(sched);
		return 1;
	}

	sched_add_status = SchedAdd(sched, (time_t)interval, task2, wrapper);
	if (1 == UIDIsBad(sched_add_status))
	{
		SchedDestroy(sched);
		return 1;		
	}

	sched_add_status = SchedAdd(sched, (time_t)interval, task3, wrapper);
	if (1 == UIDIsBad(sched_add_status))
	{
		SchedDestroy(sched);
		return 1;
	}

	return 0;
}

int SetUpSIGUSR1(sig_handler_t SignalHandler, sigaction_t *signal_handler)
{
	signal_handler->sa_sigaction = SignalHandler;
    if(0 != sigemptyset(&((*signal_handler).sa_mask)))
	{
		return 1;
	}
    signal_handler->sa_flags = SA_SIGINFO;
	if(0 != sigaction(SIGUSR1, signal_handler, NULL))
	{
		return 1;
	}

	return 0;
}

int SetUpSIGUSR2(sig_handler_t SignalHandler, sigaction_t *signal_handler)
{
	signal_handler->sa_sigaction = SignalHandler;
    if(0 != sigemptyset(&((*signal_handler).sa_mask)))
	{
		return 1;
	}
    signal_handler->sa_flags = SA_SIGINFO;
	if(0 != sigaction(SIGUSR2, signal_handler, NULL))
	{
		return 1;
	}

	return 0;
}
