/**********************************************************************
 *	This code is representing my tests of Task RC1		      	 	  *
 *	Lab: OL-79                                		                  *
 *	Author: Yonatan Vologdin 										  *
 *	Reviewer: Sharon Rottner  		  								  *
 *	Date: 22.11.19													  *
 **********************************************************************/
#include <stdio.h>		/*printf*/
#include <time.h>		/*time*/
#include <unistd.h>		/*sleep*/

#include "uid.h"		/*unique ID*/
#include "task.h"		/*task*/
#include "yoni_utils.h"	/*utilities*/

static int TaskFunc(void *action_func_param);

int TaskCreateRemoveTest();
int TaskGetIDTest();
int TaskGetTimeToRunTest();
int TaskUpdateTimeToRunTest();
int TaskIsMatchByIDTest();
int TaskRunTest();

int main()
{	
	RUN_SEQUENCE("TaskCreateRemoveTest", TaskCreateRemoveTest());
	RUN_SEQUENCE("TaskGetIDTest", TaskGetIDTest());
	RUN_SEQUENCE("TaskGetTimeToRunTest", TaskGetTimeToRunTest());
	RUN_SEQUENCE("TaskUpdateTimeToRunTest", TaskUpdateTimeToRunTest());
	RUN_SEQUENCE("TaskIsMatchByIDTest", TaskIsMatchByIDTest());
	RUN_SEQUENCE("TaskRunTest", TaskRunTest());

	return 0;
}

int TaskCreateRemoveTest()
{
	int action_func_param = 1;
	time_t interval = 10;/*seconds*/

	task_t *task = TaskCreate(interval, TaskFunc, &action_func_param);	
	printf("Check Valgrind for errors\n");
	TaskRemove(task);

	return 0;
}

int TaskGetIDTest()
{
	int action_func_param = 1;
	time_t interval = 10;/*seconds*/

	task_t *task = TaskCreate(interval, TaskFunc, &action_func_param);	
	RUN_TEST("TaskGetIDTest", 0 == UIDIsBad(TaskGetID(task)));

	TaskRemove(task);
	
	return 0;
}

int TaskGetTimeToRunTest()
{
	int action_func_param = 1;
	time_t interval = 10;/*seconds*/

	task_t *task = TaskCreate(interval, TaskFunc, &action_func_param);	
	RUN_TEST("TaskGetTimeToRunTest", (time(NULL) + 10) == TaskGetTimeToRun(task));

	TaskRemove(task);
	
	return 0;
}

int TaskUpdateTimeToRunTest()
{
	int action_func_param = 1;
	time_t interval = 10;/*seconds*/
	time_t past = time(NULL);

	task_t *task = TaskCreate(interval, TaskFunc, &action_func_param);
	sleep(2);	
	TaskUpdateTimeToRun(task);
	RUN_TEST("TaskGetTimeToRunTest", (past + 2 + interval) == TaskGetTimeToRun(task));

	TaskRemove(task);
	
	return 0;
}

int TaskIsMatchByIDTest()
{
	int action_func_param = 1;
	time_t interval = 10;/*seconds*/
	task_id_t task_id = {0};

	task_t *task = TaskCreate(interval, TaskFunc, &action_func_param);
	task_id = TaskGetID(task);

	RUN_TEST("TaskIsMatchByIDTest", TaskIsMatchByID(task_id, task));

	TaskRemove(task);

	return 0;
}

int TaskRunTest()
{
	int action_func_param = 1;
	time_t interval = 10;/*seconds*/

	task_t *task = TaskCreate(interval, TaskFunc, &action_func_param);	
	RUN_TEST("TaskRunTest", TaskRun(task));

	TaskRemove(task);
	
	return 0;
}

static int TaskFunc(void *action_func_param)
{	
	return (*(int *)action_func_param);
}
