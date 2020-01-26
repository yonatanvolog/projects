/**********************************************************************
 *	This code is representing my Watcher for WD RC1                   *
 *	Lab: OL-79                                		                  *
 *	Author: Yonatan Vologdin 										  *
 *	Reviewer: Shaddad Gomid  	  								      *
 *	Date: 20.01.20													  *
 **********************************************************************/
#include "wd_utils.h"
#include "watcher.h"

#define OTHER_PROCESS "./wd.out"

int no_response_from_wd_counter = 0;
pid_t wd_pid = {0};
int dnr_flag = 0;
sem_t *sem_thread_mmi;

void OnFail(void *wrapper)
{
	sem_t *sem_thread_mmi = sem_open("/sem_thread_mmi", 0);

	kill(wd_pid, SIGUSR2);

	((wrapper_t *)wrapper)->thread_status = 1;

	sem_post(sem_thread_mmi);
	sem_close(sem_thread_mmi);
	pthread_exit(NULL);
}

void ThreadHandler(int sig, siginfo_t *info, void *ucontext)
{
	UNUSE(sig);
	UNUSE(ucontext);

    wd_pid = info->si_pid;
	printf("\n\t\t\tTHREAD:	recieved signal from %d\n", wd_pid);
	no_response_from_wd_counter = 0;
}

void *ThreadFunc(void *wrapper)
{
	scheduler_t *sched = NULL;
	int interval = 0;
	int dead_time = 0;
	
	struct sigaction thread_signal_handler = {0};

	sem_t *sem_thread_wd1 = sem_open("/sem_thread_wd1", 0);
	sem_t *sem_thread_wd2 = sem_open("/sem_thread_wd2", O_CREAT, 0777, 0);

	printf("THREAD: deadtime env - %s\n",getenv("DEAD_TIME"));
	printf("THREAD: interval env - %s\n",getenv("INTERVAL"));

	interval = atoi(getenv("INTERVAL"));
	dead_time = atoi(getenv("DEAD_TIME"));
	((wrapper_t *)wrapper)->other_process_pid = &wd_pid;
	((wrapper_t *)wrapper)->no_response_from_other_process_counter = &no_response_from_wd_counter;
	((wrapper_t *)wrapper)->dnr_flag = &dnr_flag;
	((wrapper_t *)wrapper)->process_to_revive = WD_PROC;
	((wrapper_t *)wrapper)->interval = interval;
	((wrapper_t *)wrapper)->dead_time = dead_time;

	if (NULL == getenv("WD_ID"))
	{
		ReviveOtherProcess(wrapper);
	}

	sched = SchedCreate();
	if (NULL == sched)
	{
		sem_close(sem_thread_wd1);
		sem_close(sem_thread_wd2);
		sem_close(sem_thread_mmi);
		OnFail(wrapper);
	}

	((wrapper_t *)wrapper)->sched = sched;

	if (0 != AddTasksToSched(sched, IsOtherProcessAlive, SendSignal, CheckDNR, (time_t)interval,  wrapper))
	{
		sem_close(sem_thread_wd1);
		sem_close(sem_thread_wd2);
		sem_close(sem_thread_mmi);
		OnFail(wrapper);
	}

	if(0 != SetUpSIGUSR1(ThreadHandler, &thread_signal_handler))
	{
		SchedDestroy(sched);
		sem_close(sem_thread_wd1);
		sem_close(sem_thread_wd2);
		sem_close(sem_thread_mmi);
		OnFail(wrapper);
	}

	sem_post(sem_thread_wd1);
	sem_wait(sem_thread_wd2);

	printf("THREAD: passed semaphore block\n");

	while(0 == dnr_flag)	
	{
		if(interval * dead_time <= no_response_from_wd_counter)
		{
			no_response_from_wd_counter = 0;
			printf("\t\t\tTHREAD: REVIVING WATCHDOG\n");

			ReviveOtherProcess(wrapper);			
		}

		sem_post(sem_thread_wd1);
		sem_wait(sem_thread_wd2);
		sem_post(sem_thread_mmi);
	
		SchedRun(sched);
	}
	kill(wd_pid, SIGUSR2);

	sem_close(sem_thread_wd1);
	sem_close(sem_thread_wd2);
	sem_close(sem_thread_mmi);
	SchedDestroy(sched);
	free(wrapper);

    printf("THREAD:	sched stopped!!\n");
	sleep(1);

	pthread_exit(NULL);
}

int MMI(char **argv, int dead_time, int interval)
{
	int status = 0;
    char dead_time_buffer[30] = {0};
	char interval_buffer[30] = {0};
	pthread_t thread_id = 0;
	pthread_attr_t tattr;
	wrapper_t *mmi_return_status_wrapper = NULL;

	UNUSE(argv);

    sem_thread_mmi = sem_open("/sem_thread_mmi", O_CREAT, 0777, 0);

	printf("MMI: deadtime - %d\n",dead_time);
	printf("MMI: interval - %d\n",interval);

	sprintf(dead_time_buffer, "%d", dead_time);
	if (0 != setenv("DEAD_TIME", dead_time_buffer, 1))
	{
		sem_close(sem_thread_mmi);
		return 1;
	}

	sprintf(interval_buffer, "%d", interval);
	if(0 != setenv("INTERVAL", interval_buffer, 1))
	{
		sem_close(sem_thread_mmi);
		return 1;
	}

	mmi_return_status_wrapper = (wrapper_t *)calloc(1,sizeof(wrapper_t));
	if(NULL == mmi_return_status_wrapper)
	{
		sem_close(sem_thread_mmi);
		return 1;
	}

	if(0 != pthread_attr_init(&tattr))
	{
		free(mmi_return_status_wrapper);
		sem_close(sem_thread_mmi);
		return 1;
	}

	if(0 != pthread_attr_setdetachstate(&tattr,PTHREAD_CREATE_DETACHED))
	{
		free(mmi_return_status_wrapper);
		sem_close(sem_thread_mmi);
		return 1;
	}

    if (0 != pthread_create(&thread_id, &tattr, ThreadFunc, mmi_return_status_wrapper))
	{
		free(mmi_return_status_wrapper);
		sem_close(sem_thread_mmi);
		return 1;
	}

	sem_wait(sem_thread_mmi);
	status = mmi_return_status_wrapper->thread_status;

    return (status);
}

void DNR(void)
{
	dnr_flag = 1;
}