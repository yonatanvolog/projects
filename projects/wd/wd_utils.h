#define _POSIX_C_SOURCE 200899L	/*SA_RESTART*/
#include <stdio.h> 				/*printf*/
#include <signal.h> 			/*sigaction*/
#include <unistd.h> 			/*pid_t, sleep*/
#include <stdlib.h>				/*atoi*/
#include <fcntl.h>				/*O_CREAT*/
#include <semaphore.h>			/*semaphores*/
#include <stdio.h>				/*printf*/
#include <pthread.h>			/*pthread_create*/
#include <stdlib.h>				/*malloc*/

#include "schedule/scheduler.h"

typedef int (*task_t)(void *);
typedef void (*sig_handler_t)(int sig, siginfo_t *info, void *ucontext);
typedef struct sigaction sigaction_t;

/*General purpose wrapper for internal use*/
typedef struct wrapper_t
{
	pid_t *other_process_pid;
	int *no_response_from_other_process_counter;
	int *dnr_flag;
	int process_to_revive;
	int interval;
	int dead_time;
	scheduler_t *sched;
	int thread_status;
}wrapper_t;

#define UNUSE(x) (void)(x)
#define WD_PROC (1)
#define APP_PROC (2)

int IsOtherProcessAlive(void *wrapper);
int SendSignal(void *action_func_param);
int CheckDNR(void *wrapper);
void ReviveOtherProcess(void *wrapper);
int AddTasksToSched(scheduler_t *sched, task_t task1, task_t task2, task_t task3,
                                                 time_t interval, void *wrapper);
int SetUpSIGUSR1(sig_handler_t SignalHandler, sigaction_t *signal_handler);
int SetUpSIGUSR2(sig_handler_t SignalHandler, sigaction_t *signal_handler);
