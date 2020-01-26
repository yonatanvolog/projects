/**********************************************************************
 *	This code is representing my WD RC1				                  *
 *	Lab: OL-79                                		                  *
 *	Author: Yonatan Vologdin 										  *
 *	Reviewer: Shaddad Gomid  	  								      *
 *	Date: 20.01.20													  *
 **********************************************************************/
#include "wd_utils.h"

#define OTHER_PROCESS "./a.out"

#define TEST_WD_CRASH

int kill_myself_gracefully = 0;
pid_t thread_pid = {0};
int no_response_from_thread_counter = 0;

#ifdef TEST_WD_CRASH
int KillMyselfIn5sec(void *wrapper)
{
	static int time_till_self_kill = 5;

	UNUSE(wrapper);

	printf("\t\t\tWATCHDOG: Killing myself in %d of 5\n", (5 - time_till_self_kill));

	if (0 == time_till_self_kill)
	{
		printf("\t\t\tWATCHDOG: KILLING MYSELF NOW!\n");
	    kill(getpid(), SIGKILL);
	}

	--time_till_self_kill;

	return 1;
}
#endif

void KillMyselfHandler(int sig, siginfo_t *info, void *ucontext)
{
	UNUSE(sig);
	UNUSE(info);
	UNUSE(ucontext);

	printf("\t\t\tWATCHDOG: KILLING MYSELF SO VERY GENTLY ;) %d\n", thread_pid);
    kill_myself_gracefully = 1;
}

void WDHandler(int sig, siginfo_t *info, void *ucontext)
{
	UNUSE(sig);
	UNUSE(ucontext);

    thread_pid = info->si_pid;
    printf("\n\t\t\tWATCHDOG: recieved signal from %d\n", thread_pid);
  	no_response_from_thread_counter = 0;
}

int main()
{
	char env_buffer[30] = {0};
    struct sigaction wd_signal_handler = {0};
    struct sigaction kill_myself_handler = {0};
	wrapper_t *wrapper = NULL;
	scheduler_t *sched = NULL;
	int interval = 0;
	int dead_time = 0;
	sem_t *sem_thread_wd1 = sem_open("/sem_thread_wd1", O_CREAT, 0777, 0);
	sem_t *sem_thread_wd2 = sem_open("/sem_thread_wd2", 0);
#ifdef TEST_WD_CRASH
ilrd_uid_t sched_add_status = {0};
#endif
	printf("WATCHDONG: deadtime env - %s\n",getenv("DEAD_TIME"));
	printf("WATCHDONG: interval env - %s\n",getenv("INTERVAL"));

	/*sets ENV to indicate if it's the program's first run*/
	sprintf(env_buffer, "%d", getpid());
	if(0 != setenv("WD_ID", env_buffer, 1))
	{
		exit(1);
	}

    wrapper = (wrapper_t *)malloc(sizeof(wrapper_t));
	if (NULL == wrapper)
	{
		exit(1);
	}

	interval = atoi(getenv("INTERVAL"));
	dead_time = atoi(getenv("DEAD_TIME"));

    sched = SchedCreate();
	if (NULL == sched)
	{
		exit(1);
	}

	wrapper->dead_time = dead_time;
	wrapper->interval = interval;
	wrapper->sched = sched;
	wrapper->no_response_from_other_process_counter = &no_response_from_thread_counter;
	wrapper->other_process_pid = &thread_pid;
	wrapper->dnr_flag = &kill_myself_gracefully;
	wrapper->process_to_revive = APP_PROC;

	if (0 != AddTasksToSched(sched, IsOtherProcessAlive, SendSignal, CheckDNR, (time_t)interval,  wrapper))
	{
		SchedDestroy(sched);
		exit(1);
	}

#ifdef TEST_WD_CRASH
	sched_add_status = SchedAdd(sched, (time_t)interval, KillMyselfIn5sec, wrapper);
	if (1 == UIDIsBad(sched_add_status))
	{
		printf("\n\n\n\nFailed to set TEST_WD_CRASH\n\n\n\n");
		return 1;
	}
#endif

	if(0 != SetUpSIGUSR1(WDHandler, &wd_signal_handler))
	{
		SchedDestroy(sched);
		exit(1);
	}

	if(0 != SetUpSIGUSR2(KillMyselfHandler, &kill_myself_handler))
	{
		SchedDestroy(sched);
		exit(1);
	}

	sem_post(sem_thread_wd2);
	sem_wait(sem_thread_wd1);
	
	printf("WATCHDOG: passed semaphore block\n");

	while(0 == kill_myself_gracefully)	
	{
		if(interval * dead_time <= no_response_from_thread_counter)
		{
			no_response_from_thread_counter = 0;
			
			printf("\t\t\tWATCHDOG: REVIVING THREAD\n");

			ReviveOtherProcess(wrapper);			
		}
		sem_post(sem_thread_wd2);
		sem_wait(sem_thread_wd1);

		SchedRun(sched);
	}

	sem_close(sem_thread_wd1);
	sem_close(sem_thread_wd2);
	SchedDestroy(sched);
	free(wrapper);

    printf("WATCHDOG: WD sched stopped garcefully!!\n");
  
    return 0;
}