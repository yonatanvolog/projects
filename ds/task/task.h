/****************************************************************************
*                                                                           *
*                               OL79 - 18/11/19                             *
*                                                                           *	
*                               Task                                        *
*                                                                           *
*****************************************************************************/
#ifndef ILRD_TASK_H
#define ILRD_TASK_H
#include <time.h>   /* time_t */
#include <stddef.h> /* size_t */
#include "uid.h"

typedef int(*task_func)(void *action_func_param);
typedef struct task task_t;
typedef ilrd_uid_t task_id_t;


/*	This Function creates a Task
*	The function receives Interval, action_func and action_func_param.
*	Action_func will return 0 if task is completed and no reaccurence is needed, 
*   and return 1 if reaccurence is needed.
*	Return Value: task_t* which is a pointer to the struct.
*   if the function fails (UID failed / malloc failed), it returns NULL. 			  
*	Notice: user must supply a valid action_func and a positive interval */
task_t *TaskCreate(time_t interval, task_func action_func, void* action_func_param);

/*  This function removes a task which was only created by TaskCreate. 
*	the function de-alloctes memory. */
void TaskRemove(task_t *task);

/*	The function recieves a task and returns correclating UID,
*	Notice: The task cannot be NULL */
task_id_t TaskGetID(const task_t *task);

/* This function updates time_to_run of task, according to interval + current time.
   This function is recommended for runtime use.
   Notice: The task cannot be NULL */
void TaskUpdateTimeToRun(task_t *task);

/* This function returns the value of time_to_run, i.e. when this function 
   is schedualed to run the next time.
   Notice: The task cannot be NULL */
time_t TaskGetTimeToRun(task_t *task);

/* This function recieves task_id_t and task_t. 
   Func will compare the id to the task, and returns 1 if match, or 0 if not.
   Notice: The task cannot be NULL */
int TaskIsMatchByID(task_id_t id_to_find, task_t *task);

/* This function recieves task and runs the action_func of the task. 
   Function will return the value received from the action_func.
   If the action function that the task ran returns 1 (meaning - it want to repeat
   itself), TaskRun will call TaskUpdateTimeToRun in order to update the next 
   runtime.
   Notice: The task cannot be NULL */
int TaskRun(task_t *task);


#endif
