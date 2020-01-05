/**********************************************************************
 *	This code is representing my functions of Scheduler RC2	          *
 *	Lab: OL-79                                		                  *
 *	Author: Yonatan Vologdin 										  *
 *	Reviewer: Sharon Rottner 	  								 	  *
 *	Date: 24.11.19													  *
 **********************************************************************/
#include <stdio.h>		/*printf*/
#include <stdlib.h>		/*malloc*/
#include <assert.h>		/*assert*/
#include <unistd.h>		/*sleep*/

#include "pq.h"			/*priority queue data structure*/
#include "uid.h"		/*unique ID*/
#include "task.h"		/*task*/
#include "scheduler.h"	/*scheduler*/
#include "../../yoni_utils.h"

struct scheduler
{
	p_queue_t *pq;
	int stop_flag;
	task_t *running_task;
	int has_removed_itself;
};

static int CompareFuncIMP(const void *new_data, const void *src_data, void *param)
{	
	UNUSE(param);
	
	if ((TaskGetTimeToRun((task_t *)new_data)) > (TaskGetTimeToRun((task_t *)src_data)))
	{
		return (-1);
	}

	else if ((TaskGetTimeToRun((task_t *)src_data)) > (TaskGetTimeToRun((task_t *)new_data)))
	{
		return (1);
	}

	return 0; /*src_data == new_data*/
}

scheduler_t *SchedCreate(void)
{	
	scheduler_t *new_schedule = (scheduler_t *)malloc(sizeof(scheduler_t));
	if (NULL == new_schedule)
	{	
		return NULL;
	}
	
	new_schedule->pq = PQCreate(NULL, CompareFuncIMP);
	if (NULL == new_schedule->pq)
	{	
		return NULL;
	}

	new_schedule->stop_flag = 0;
	new_schedule->running_task = NULL;
	new_schedule->has_removed_itself = 0;

	return new_schedule;
}

void SchedDestroy(scheduler_t *scheduler)
{
	assert(scheduler);
	
	SchedClear(scheduler);
	PQDestroy(scheduler->pq);
	free(scheduler);
}

ilrd_uid_t SchedAdd(scheduler_t *scheduler, time_t interval, action_func action,
 														void *action_func_param)
{
	task_t *new_task = NULL;

	assert(scheduler);

	new_task = TaskCreate(interval, action, action_func_param);
	if (NULL == new_task)
	{
		return BAD_UID;
	}

	if (PQEnqueue(scheduler->pq, new_task))
	{
		TaskRemove(new_task);

		return BAD_UID;
	}

	return (TaskGetID(new_task));
}

static int IsMatchIMP(const void *new_data, const void *param)
{
	return (TaskIsMatchByID(*(task_id_t *)new_data, (task_t *)param));
}

int SchedRemove(scheduler_t *scheduler, ilrd_uid_t event_id)
{	
	task_t *task_to_remove = NULL;

	assert(scheduler);

	task_to_remove = (task_t *)PQErase(&event_id, scheduler->pq, IsMatchIMP);	
	if (NULL == task_to_remove) /*didnt find task in pq*/
	{
		if ((scheduler->running_task) && 
			(TaskIsMatchByID(event_id, scheduler->running_task)))
		{		
			scheduler->has_removed_itself = 1;

			return 0;
		}

		return 1;
	}
	TaskRemove(task_to_remove);
		
	return 0;
}

size_t SchedSize(const scheduler_t *scheduler)
{
	size_t sched_size = 0;	
	
	assert(scheduler);

 	sched_size = PQSize(scheduler->pq);

	if (NULL != (scheduler->running_task))
	{
		++sched_size;
	}

	return sched_size;
}

int SchedIsEmpty(const scheduler_t *scheduler)
{
	assert(scheduler);

	if (0 == (SchedSize(scheduler)))
	{
		return 1;
	}
	
	return 0;
}

enum result_status SchedRun(scheduler_t *scheduler)
{	
	int is_repeat = 0;

	assert(scheduler);

	scheduler->stop_flag = 0;
	while(!(scheduler->stop_flag))
	{
		scheduler->running_task = PQDequeue(scheduler->pq);
		sleep(MAX((TaskGetTimeToRun(scheduler->running_task) - time(NULL)), 0));
		is_repeat = TaskRun(scheduler->running_task);

		if (!(scheduler->has_removed_itself) && is_repeat)
		{
			TaskUpdateTimeToRun(scheduler->running_task);
			
			if (PQEnqueue(scheduler->pq, scheduler->running_task))
			{
				scheduler->stop_flag = 1;
				scheduler->running_task = NULL;

				return ENQUEUE_FAILED;
			}
			scheduler->running_task = NULL;
		}

		else
		{
			TaskRemove(scheduler->running_task);
			scheduler->has_removed_itself = 0;
			scheduler->running_task = NULL;
		}

		if (SchedIsEmpty(scheduler))
		{
			scheduler->stop_flag = 1;

			return SCHEDULER_EMPTY;
		}
	}

	return STOPPED_SUCCESSFULLY;
}

void SchedStop(scheduler_t *scheduler)
{
	assert(scheduler);

	scheduler->stop_flag = 1;
}

void SchedClear(scheduler_t *scheduler)
{
	assert(scheduler);
	
	while (!PQIsEmpty(scheduler->pq))
	{
		TaskRemove((task_t *)PQDequeue(scheduler->pq));
	}
	
	if (NULL != (scheduler->running_task))
	{
		scheduler->has_removed_itself = 1;
	}
}
