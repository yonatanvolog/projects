/****************************************************************************
*                                                                           *
*                               OL79 - 19/11/19                             *
*                                                                           *	
*                                  Scheduler                                *
*                                                                           *
*****************************************************************************/
#ifndef ILRD_SCHEDULER_H
#define ILRD_SCHEDULER_H
#include <stddef.h> /* size_t */
#include <time.h>   /* time_t */
#include "uid.h"

enum result_status
{
	STOPPED_SUCCESSFULLY = 0,
	ENQUEUE_FAILED = 1,
	SCHEDULER_EMPTY = 2
};

typedef struct scheduler scheduler_t;
typedef int (*action_func)(void *action_func_param);

/*	This Function creates a Task Scheduler. 
*	The task scheduler recieves events, and runs them by time.	
*	Return Value: a pointer to the Task Scheduler. 
*	if the function fails, it returns NULL. 
*/
scheduler_t *SchedCreate(void);

/*  This function destroys a Task Scheduler which was created only by SchedCreate. 
*	the function also frees all the memory that was allocated.
*   Calling with any other Task Scheduler will result in undefined behavior. 
*/
void SchedDestroy(scheduler_t *scheduler);

/*	The function adds a new event to the Task Scheduler.
*	The function recieves:
*	1. A pointer to scheduler_t.
*	2. A time_t interval. The actual run time will only be set according to the 
*	   time of calling SchedAdd. For example, if the interval is 5 hours, and you
*	   call SchedAdd at 13:00, the actual run time will be 18:00.
*	3. A pointer to a function that	the event will run.
*	4. An argument that will be passed to the action function, if needed.
*	 
*	Return Value: The event id of the event added. in case of failure, the
*	function will return BADUID (you can check the uid, using the lovely 
*	function UIDIsBad.
*	scheduler must be a valid Task Scheduler. If the function receives a
*	NULL Task Scheduler, it will return NULL.
*
*	Notice: action_func cannot be NULL.
*/
ilrd_uid_t SchedAdd(scheduler_t *scheduler, time_t interval, action_func action,
 void *action_func_param);

/*  This function removes an event from the Task Scheduler.
*
*   Return Value: 1 - if the function fails (for example, if the event is
*				      not valid or no matching task was found). 
*				  0 - if event was successfully removed.
*   scheduler must be a valid Task Scheduler. if the function receives a
*   NULL Task Scheduler, it will return 1.
*/   
int SchedRemove(scheduler_t *scheduler, ilrd_uid_t event_id);


/* This function returns the size (number of events) of the scheduler. 
*  scheduler must be a valid Task Scheduler. 
*  Passing a NULL Task Scheduler will result in undefined behavior. 
*/   
size_t SchedSize(const scheduler_t *scheduler);

/* This function checks if the scheduler is empty. 
*   Return values: 0 - not empty, 1 - empty. 
*   Passing a NULL Task Scheduler will result in undefined behavior. 
*/   
int SchedIsEmpty(const scheduler_t *scheduler);

/*
*	This function will start running the task scheduler, which means it 
*	will start running events, according to their time due.
*   If the task to run is overdue, the scheduler will run it immediately
*	
*	Return Values: 1 - if the function fails or terminated unexpectedly. 
*				   0 - if scheduler was successfully ran and terminated.
*	
*/		
enum result_status SchedRun(scheduler_t *scheduler);

/*
*	This function will stop the task scheduler run_time (started with SchedRun), 
*	which means it will stop run events, even if their time is due.
*	
*	Notice: calling SchedStop must be after calling SchedRun. otherwise, it will 
*	result in undefined behavior 
*/
void SchedStop(scheduler_t *scheduler);

/*  
*	This function clears all events from the scheduler, but it does not destroy
*	the scheduler itself.
*	
*	Notice: attempting to clear a non valid scheduler will result in 
*   undefined behavior. 
*/    
void SchedClear(scheduler_t *scheduler);

#endif
