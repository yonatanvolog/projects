/**********************************************************************
 *	This code is representing my tests of Scheduler RC1		       	  *
 *	Lab: OL-79                                		                  *
 *	Author: Yonatan Vologdin 										  *
 *	Reviewer: Sharon Rottner  		  								  *
 *	Date: 23.11.19													  *
 **********************************************************************/
#include <stdio.h>		/*printf*/
#include <time.h>		/*time*/
#include <unistd.h>		/*sleep*/
#include <stdlib.h>		/*malloc*/

#include "uid.h"		/*unique ID*/
#include "task.h"		/*task*/
#include "scheduler.h"	/*schedule*/
#include "../../yoni_utils.h"

typedef struct remove_wrapper
{
	scheduler_t *main_schedule;
	ilrd_uid_t *uid;
}remove_wrapper_t;

static int TaskFunc(void *action_func_param);
static int PrintHi(void *action_func_param);
static int PrintYoni(void *action_func_param);
static int PrintWhats(void *action_func_param);
static int PrintUp(void *action_func_param);
static int EmptyFunc(void *action_func_param);

static int MySchedCreate(void *action_func_param);
static int MySchedAdd(void *action_func_param);
static int MySchedSize(void *action_func_param);
static int MySchedIsEmpty(void *action_func_param);
static int MySchedStop(void *action_func_param);
static int MySchedClear(void *action_func_param);
static int MySchedRemove(void *action_func_param);

int SchedCreateDestroyTest();
int SchedAddIsEmptyTest();
int SchedRemoveTest();
int SchedSizeTest();
int SchedClearTest();
int SchedRunMyPrintTest();
int SchedRunSchedCreateTest();
int SchedRunSchedAddTest();
int SchedRunSchedRemoveTest();
int SchedRunSchedSizeTest();
int SchedRunSchedIsEmptyTest();
int SchedRunSchedStopTest();
int SchedRunSchedClearTest();

int main()
{
	RUN_SEQUENCE("SchedCreateDestroyTest", SchedCreateDestroyTest());
	RUN_SEQUENCE("SchedAddIsEmptyTest", SchedAddIsEmptyTest());
	RUN_SEQUENCE("SchedRemoveTest", SchedRemoveTest());
	RUN_SEQUENCE("SchedSizeTest", SchedSizeTest());
	RUN_SEQUENCE("SchedClearTest", SchedClearTest());
	RUN_SEQUENCE("SchedRunMyPrintTest", SchedRunMyPrintTest());
	RUN_SEQUENCE("SchedRunSchedCreateTest", SchedRunSchedCreateTest());	
	RUN_SEQUENCE("SchedRunSchedAddTest", SchedRunSchedAddTest());	
	RUN_SEQUENCE("SchedRunSchedRemoveTest", SchedRunSchedRemoveTest());	
	RUN_SEQUENCE("SchedRunSchedSizeTest", SchedRunSchedSizeTest());	
	RUN_SEQUENCE("SchedRunSchedIsEmptyTest", SchedRunSchedIsEmptyTest());	
	RUN_SEQUENCE("SchedRunSchedStopTest", SchedRunSchedStopTest());	
	RUN_SEQUENCE("SchedRunSchedClearTest", SchedRunSchedClearTest());	

	return 0;
}

int SchedCreateDestroyTest()
{
	SchedDestroy(SchedCreate());
	printf("Check Valgrind for errors\n");

	return 0;
}

int SchedAddIsEmptyTest()
{	
	scheduler_t *schedule = SchedCreate();
	int action_func_param = 1;
	time_t interval = 10;/*seconds*/
	RUN_TEST("SchedAddIsEmptyTest1", 1 == SchedIsEmpty(schedule));
	SchedAdd(schedule, interval, TaskFunc, &action_func_param);
	RUN_TEST("SchedAddIsEmptyTest2", 0 == SchedIsEmpty(schedule));

	SchedDestroy(schedule);

	return 0;
}

int SchedRemoveTest()
{	
	scheduler_t *schedule = SchedCreate();
	int action_func_param = 1;
	ilrd_uid_t event_id = {0};

	time_t interval = 10;

	event_id = SchedAdd(schedule, interval, TaskFunc, &action_func_param);
	RUN_TEST("SchedRemoveTest1", 0 == SchedRemove(schedule, event_id));
	RUN_TEST("SchedRemoveTest2", 1 == SchedIsEmpty(schedule));

	SchedDestroy(schedule);

	return 0;
}

int SchedSizeTest()
{
	scheduler_t *schedule = SchedCreate();

	int action_func_param1 = 1;
	int action_func_param2 = 1;
	int action_func_param3 = 1;
	int action_func_param4 = 1;
	int action_func_param5 = 1;

	ilrd_uid_t event_id1 = {0};
	ilrd_uid_t event_id2 = {0};
	ilrd_uid_t event_id3 = {0};
	ilrd_uid_t event_id4 = {0};
	ilrd_uid_t event_id5 = {0};

	time_t interval = 10;

	RUN_TEST("SchedSizeTest1", 0 == SchedSize(schedule));
		
	event_id1 = SchedAdd(schedule, interval, TaskFunc, &action_func_param1);
	event_id2 = SchedAdd(schedule, interval, TaskFunc, &action_func_param2);
	event_id3 = SchedAdd(schedule, interval, TaskFunc, &action_func_param3);
	event_id4 = SchedAdd(schedule, interval, TaskFunc, &action_func_param4);
	event_id5 = SchedAdd(schedule, interval, TaskFunc, &action_func_param5);

	RUN_TEST("SchedSizeTest2", 5 == SchedSize(schedule));
	SchedRemove(schedule, event_id1);
	RUN_TEST("SchedSizeTest3", 4 == SchedSize(schedule));
	SchedRemove(schedule, event_id2);
	SchedRemove(schedule, event_id3);
	SchedRemove(schedule, event_id4);
	RUN_TEST("SchedSizeTest4", 1 == SchedSize(schedule));
	SchedRemove(schedule, event_id5);
	RUN_TEST("SchedSizeTest5", 0 == SchedSize(schedule));
	RUN_TEST("SchedSizeTest6", 1 == SchedRemove(schedule, event_id5));

	SchedDestroy(schedule);

	return 0;
}

int SchedClearTest()
{
	scheduler_t *schedule = SchedCreate();

	int action_func_param1 = 1;
	int action_func_param2 = 1;
	int action_func_param3 = 1;
	int action_func_param4 = 1;
	int action_func_param5 = 1;

	time_t interval = 10;

	SchedAdd(schedule, interval, TaskFunc, &action_func_param1);
	SchedAdd(schedule, interval, TaskFunc, &action_func_param2);
	SchedAdd(schedule, interval, TaskFunc, &action_func_param3);
	SchedAdd(schedule, interval, TaskFunc, &action_func_param4);
	SchedAdd(schedule, interval, TaskFunc, &action_func_param5);

	SchedClear(schedule);

	RUN_TEST("SchedClearTest", 0 == SchedSize(schedule));

	SchedDestroy(schedule);

	return 0;
}

int SchedRunMyPrintTest()
{
	scheduler_t *schedule = SchedCreate();

	time_t interval1 = 1;
	time_t interval2 = 2;
	time_t interval3 = 3;
	time_t interval4 = 4;

	SchedAdd(schedule, interval1, PrintHi, NULL);
	SchedAdd(schedule, interval2, PrintYoni, NULL);
	SchedAdd(schedule, interval3, PrintWhats, NULL);
	SchedAdd(schedule, interval4, PrintUp, NULL);

	SchedRun(schedule);

	RUN_TEST("SchedRunMyPrintTest", 1 == SchedIsEmpty(schedule));

	SchedDestroy(schedule);

	return 0;
}

int SchedRunSchedCreateTest()
{
	int res = 0;

	scheduler_t *main_schedule = SchedCreate();

	FILE *file = NULL;

	time_t interval = 1;

	/*create flag file*/
	file  = fopen("sched_d_file", "w"); 
	SchedAdd(main_schedule, interval, MySchedCreate, main_schedule);
	res = SchedRun(main_schedule);
	RUN_TEST("SchedRunReturn_WithFile_Test2\t", 2 == res);	/*run returns SCHEDULER_EMPTY*/

	/*remove flag file*/
	fclose(file);
	remove("sched_d_file");

	SchedAdd(main_schedule, interval, MySchedCreate, main_schedule);
	SchedAdd(main_schedule, interval, EmptyFunc, NULL);
	res = SchedRun(main_schedule);
	RUN_TEST("SchedRunReturn_WithoutFile_Test4", 0 == res);	/*run returns STOPPED_SUCCESSFULLY*/

	SchedDestroy(main_schedule);

	return 0;
}

static int MySchedCreate(void *action_func_param)
{
	/*if file exists*/
	if (0 == access("sched_d_file", F_OK )) 
	{
		scheduler_t *secondary_schedule = SchedCreate();
		RUN_TEST("SchedRunSched_Test1\t\t", 0 == SchedIsEmpty(action_func_param));
		SchedDestroy(secondary_schedule);
	} 
	/*if file doesnt exist*/
	else 	
	{
		scheduler_t *secondary_schedule = SchedCreate();	
		RUN_TEST("SchedRunSched_Test3\t\t", 0 == SchedIsEmpty(action_func_param));
		SchedStop((scheduler_t *)action_func_param);
		SchedDestroy(secondary_schedule);
	}
 
	return 0;
}

int SchedRunSchedAddTest()
{
	int res = 0;

	scheduler_t *main_schedule = SchedCreate();

	FILE *file = NULL;

	time_t interval = 1;

	/*create flag file*/
	file  = fopen("sched_d_file", "w"); 
	SchedAdd(main_schedule, interval, MySchedAdd, main_schedule);
	res = SchedRun(main_schedule);
	RUN_TEST("SchedRunReturn_WithFile_Test2\t", 2 == res);	/*run returns SCHEDULER_EMPTY*/

	/*remove flag file*/
	fclose(file);
	remove("sched_d_file");

	SchedAdd(main_schedule, interval, MySchedAdd, main_schedule);
	SchedAdd(main_schedule, interval, EmptyFunc, NULL);
	res = SchedRun(main_schedule);
	RUN_TEST("SchedRunReturn_WithoutFile_Test4", 0 == res);	/*run returns STOPPED_SUCCESSFULLY*/

	SchedDestroy(main_schedule);

	return 0;
}

static int MySchedAdd(void *action_func_param)
{
	time_t interval = 1;

	/*if file exists*/
	if (0 == access("sched_d_file", F_OK )) 
	{
		SchedAdd(action_func_param, interval, EmptyFunc, NULL);
		RUN_TEST("SchedRunSched_Test1\t\t", 0 == SchedIsEmpty(action_func_param));
	} 
	/*if file doesnt exist*/
	else 	
	{
		SchedAdd(action_func_param, interval, EmptyFunc, NULL);
		RUN_TEST("SchedRunSched_Test3\t\t", 0 == SchedIsEmpty(action_func_param));
		SchedStop((scheduler_t *)action_func_param);
	}
 
	return 0;
}

int SchedRunSchedRemoveTest()
{
	int res = 0;
	FILE *file = NULL;
	time_t interval = 1;
	scheduler_t *main_schedule = SchedCreate();

	remove_wrapper_t *wrapper = NULL;
	wrapper = (remove_wrapper_t *)malloc(sizeof(remove_wrapper_t));
	if (NULL == wrapper)
	{
		printf("remove_wrapper_t malloc failed\n");
	}
	wrapper->main_schedule = main_schedule; 
	wrapper->uid = (ilrd_uid_t *)malloc(sizeof(ilrd_uid_t));
	if (NULL == wrapper->uid)
	{
		printf("ilrd_uid_t malloc failed\n");
	}

	/*create flag file*/
	file  = fopen("sched_d_file", "w"); 
	*wrapper->uid = SchedAdd(main_schedule, interval, MySchedRemove, wrapper);
	res = SchedRun(main_schedule);
	RUN_TEST("SchedRunReturn_WithFile_Test2\t", 2 == res);	/*run returns SCHEDULER_EMPTY*/

	/*remove flag file*/
	fclose(file);
	remove("sched_d_file");
	*wrapper->uid = SchedAdd(main_schedule, interval, MySchedRemove, wrapper);
					SchedAdd(main_schedule, interval, EmptyFunc, NULL);
	res = SchedRun(main_schedule);
	RUN_TEST("SchedRunReturn_WithoutFile_Test4", 0 == res);	/*run returns STOPPED_SUCCESSFULLY*/

	free(wrapper->uid);
	free(wrapper);
	SchedDestroy(main_schedule);

	return 0;
}

static int MySchedRemove(void *action_func_param)
{
	remove_wrapper_t *wrapper = (remove_wrapper_t *)action_func_param;

	/*if file exists*/
	if (0 == access("sched_d_file", F_OK )) 
	{
		SchedRemove(wrapper->main_schedule, *wrapper->uid);
		RUN_TEST("SchedRunSched_Test1\t\t", 1 == SchedSize(wrapper->main_schedule));
	} 
	/*if file doesnt exist*/
	else 	
	{
		SchedRemove(wrapper->main_schedule, *wrapper->uid);
		RUN_TEST("SchedRunSched_Test3\t\t", 2 == SchedSize(wrapper->main_schedule));
		SchedStop(wrapper->main_schedule);
	}
 
	return 0;
}

int SchedRunSchedSizeTest()
{
	int res = 0;

	scheduler_t *main_schedule = SchedCreate();

	FILE *file = NULL;

	time_t interval = 1;

	/*create flag file*/
	file  = fopen("sched_d_file", "w"); 
	SchedAdd(main_schedule, interval, MySchedSize, main_schedule);
	res = SchedRun(main_schedule);
	RUN_TEST("SchedRunReturn_WithFile_Test2\t", 2 == res);	/*run returns SCHEDULER_EMPTY*/

	/*remove flag file*/
	fclose(file);
	remove("sched_d_file");

	SchedAdd(main_schedule, interval, MySchedSize, main_schedule);
	SchedAdd(main_schedule, interval, EmptyFunc, NULL);
	res = SchedRun(main_schedule);
	RUN_TEST("SchedRunReturn_WithoutFile_Test4", 0 == res);	/*run returns STOPPED_SUCCESSFULLY*/

	SchedDestroy(main_schedule);

	return 0;
}

static int MySchedSize(void *action_func_param)
{
	/*if file exists*/
	if (0 == access("sched_d_file", F_OK )) 
	{
		RUN_TEST("SchedRunSched_Test1\t\t", 1 == SchedSize(action_func_param));
	} 
	/*if file doesnt exist*/
	else 	
	{
		RUN_TEST("SchedRunSched_Test3\t\t", 2 == SchedSize(action_func_param));
		SchedStop((scheduler_t *)action_func_param);
	}
 
	return 0;
}

int SchedRunSchedIsEmptyTest()
{
	int res = 0;

	scheduler_t *main_schedule = SchedCreate();

	FILE *file = NULL;

	time_t interval = 1;

	/*create flag file*/
	file  = fopen("sched_d_file", "w"); 
	SchedAdd(main_schedule, interval, MySchedIsEmpty, main_schedule);
	res = SchedRun(main_schedule);
	RUN_TEST("SchedRunReturn_WithFile_Test2\t", 2 == res);	/*run returns SCHEDULER_EMPTY*/

	/*remove flag file*/
	fclose(file);
	remove("sched_d_file");

	SchedAdd(main_schedule, interval, MySchedIsEmpty, main_schedule);
	SchedAdd(main_schedule, interval, EmptyFunc, NULL);
	res = SchedRun(main_schedule);
	RUN_TEST("SchedRunReturn_WithoutFile_Test4", 0 == res);	/*run returns STOPPED_SUCCESSFULLY*/

	SchedDestroy(main_schedule);

	return 0;
}

static int MySchedIsEmpty(void *action_func_param)
{
	/*if file exists*/
	if (0 == access("sched_d_file", F_OK )) 
	{
		RUN_TEST("SchedRunSched_Test1\t\t", 0 == SchedIsEmpty(action_func_param));
	} 
	/*if file doesnt exist*/
	else 	
	{
		RUN_TEST("SchedRunSched_Test3\t\t", 0 == SchedIsEmpty(action_func_param));
		SchedStop((scheduler_t *)action_func_param);
	}
 
	return 0;
}

int SchedRunSchedStopTest()
{
	int res = 0;

	scheduler_t *main_schedule = SchedCreate();

	FILE *file = NULL;

	time_t interval = 1;

	/*create flag file*/
	file  = fopen("sched_d_file", "w"); 
	SchedAdd(main_schedule, interval, MySchedStop, main_schedule);
	res = SchedRun(main_schedule);
	RUN_TEST("SchedRunReturn_WithFile_Test2\t", 2 == res);	/*run returns SCHEDULER_EMPTY*/

	/*remove flag file*/
	fclose(file);
	remove("sched_d_file");

	SchedAdd(main_schedule, interval, MySchedStop, main_schedule);
	SchedAdd(main_schedule, interval, EmptyFunc, NULL);
	res = SchedRun(main_schedule);
	RUN_TEST("SchedRunReturn_WithoutFile_Test4", 0 == res);	/*run returns STOPPED_SUCCESSFULLY*/

	SchedDestroy(main_schedule);

	return 0;
}

static int MySchedStop(void *action_func_param)
{
	/*if file exists*/
	if (0 == access("sched_d_file", F_OK )) 
	{
		RUN_TEST("SchedRunSched_Test1\t\t", 0 == SchedIsEmpty(action_func_param));
	} 
	/*if file doesnt exist*/
	else 	
	{
		RUN_TEST("SchedRunSched_Test3\t\t", 0 == SchedIsEmpty(action_func_param));
		SchedStop((scheduler_t *)action_func_param);
	}
 
	return 0;
}

int SchedRunSchedClearTest()
{
	int res = 0;

	scheduler_t *main_schedule = SchedCreate();

	FILE *file = NULL;

	time_t interval = 1;

	/*create flag file*/
	file  = fopen("sched_d_file", "w"); 
	SchedAdd(main_schedule, interval, MySchedClear, main_schedule);
	res = SchedRun(main_schedule);
	RUN_TEST("SchedRunReturn_WithFile_Test2\t", 2 == res);	/*run returns SCHEDULER_EMPTY*/

	/*remove flag file*/
	fclose(file);
	remove("sched_d_file");

	SchedAdd(main_schedule, interval, MySchedClear, main_schedule);
	SchedAdd(main_schedule, interval, EmptyFunc, NULL);
	res = SchedRun(main_schedule);
	RUN_TEST("SchedRunReturn_WithoutFile_Test4", 2 == res);	/*run returns SCHEDULER_EMPTY*/

	SchedDestroy(main_schedule);

	return 0;
}

static int MySchedClear(void *action_func_param)
{
	/*if file exists*/
	if (0 == access("sched_d_file", F_OK )) 
	{
		SchedClear((scheduler_t *)action_func_param);
		RUN_TEST("SchedRunSched_Test1\t\t", 1 == SchedSize(action_func_param));
	} 
	/*if file doesnt exist*/
	else 	
	{
		SchedStop((scheduler_t *)action_func_param);
		SchedClear((scheduler_t *)action_func_param);
		/*size is going to be 1 because clear deleted the other non running task*/
		RUN_TEST("SchedRunSched_Test3\t\t", 1 == SchedSize(action_func_param));
	}
 
	return 0;
}






static int TaskFunc(void *action_func_param)
{	
	return (*(int *)action_func_param);
}

static int PrintHi(void *action_func_param)
{
	UNUSE(action_func_param);
	printf("Hi ");	
	fflush(stdout);

	return 0;
}

static int PrintYoni(void *action_func_param)
{
	UNUSE(action_func_param);
	printf("Yoni ");	
	fflush(stdout);

	return 0;
}

static int PrintWhats(void *action_func_param)
{
	UNUSE(action_func_param);
	printf("Whats ");	
	fflush(stdout);

	return 0;
}

static int PrintUp(void *action_func_param)
{
	UNUSE(action_func_param);
	printf("Up?\n");	
	fflush(stdout);

	return 0;
}

static int EmptyFunc(void *action_func_param)
{
	UNUSE(action_func_param);
	
	return 0;
}
