/**********************************************************************
 *	This code is representing my tests of DS_PQ RC2		        	  *
 *	Lab: OL-79                                		                  *
 *	Author: Yonatan Vologdin 										  *
 *	Reviewer: Dvir Breitbart  		  								  *
 *	Date: 19.11.19													  *
 **********************************************************************/
#include <stdio.h>	/*printf*/

#include "pq.h"		/*priority queue data structure*/

#define	UNUSE(var) ((void)(var))

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"

#define RUN_SEQUENCE(test_name, test)       \
printf("\n******* %s *******\n\n",test_name);\
test								          \

#define RUN_TEST(test_name, result) \
(printf("%s %s\n", test_name, (1 == (result)) ? KGRN"\tPassed"KNRM : KRED"\tFailed"KNRM))

int CreateDestroyTest();
int EnqueuePeekTest();
int DequeueIsEmptyTest();
int SizeTest();
int ClearTest();
int EraseTest();

static int CompareFuncInt(const void *new_data, const void *src_data, void *param);
static int IsMatch(const void *new_data, const void *param);

int main()
{
	RUN_SEQUENCE("CreateDestroyTest", CreateDestroyTest());
	RUN_SEQUENCE("EnqueuePeekTest", EnqueuePeekTest());	
	RUN_SEQUENCE("DequeueIsEmptyTest", DequeueIsEmptyTest());	
	RUN_SEQUENCE("SizeTest", SizeTest());	
	RUN_SEQUENCE("ClearTest", ClearTest());	
	RUN_SEQUENCE("EraseTest", EraseTest());	

	return 0;
}

int CreateDestroyTest()
{
	p_queue_t *pq1 = PQCreate(NULL, CompareFuncInt);
	printf("Check Valgrind for errors\n");
	PQDestroy(pq1);

	return 0;
}

int EnqueuePeekTest()
{
	int data1 = 7;
	int data2 = 3;
	int data3 = 5;	

	p_queue_t *pq1 = PQCreate(NULL, CompareFuncInt);
	PQEnqueue(pq1, &data1);
	RUN_TEST("EnqueueTest1", 7 == *(int *)(PQPeek(pq1)));
	PQEnqueue(pq1, &data2);
	RUN_TEST("EnqueueTest2", 3 == *(int *)(PQPeek(pq1)));
	PQEnqueue(pq1, &data3);
	RUN_TEST("EnqueueTest3", 3 == *(int *)(PQPeek(pq1)));

	PQDestroy(pq1);
	
	return 0;
}

int DequeueIsEmptyTest()
{
	int data1 = 7;
	int data2 = 5;
	int data3 = 3;	

	p_queue_t *pq1 = PQCreate(NULL, CompareFuncInt);

	RUN_TEST("IsEmptyTest1", 1 == PQIsEmpty(pq1));
	PQEnqueue(pq1, &data1);
	RUN_TEST("IsEmptyTest2", 0 == PQIsEmpty(pq1));
	PQEnqueue(pq1, &data2);
	PQEnqueue(pq1, &data3);

	RUN_TEST("DequeueTest1", 3 == *(int *)(PQPeek(pq1)));
	PQDequeue(pq1);
	RUN_TEST("DequeueTest2", 5 == *(int *)(PQPeek(pq1)));
	PQDequeue(pq1);
	RUN_TEST("DequeueTest3", 7 == *(int *)(PQPeek(pq1)));
	PQDequeue(pq1);
	RUN_TEST("IsEmptyTest3", 1 == PQIsEmpty(pq1));

	PQDestroy(pq1);
	
	return 0;
}

int SizeTest()
{
	int data1 = 7;
	int data2 = 5;
	int data3 = 3;	

	p_queue_t *pq1 = PQCreate(NULL, CompareFuncInt);

	RUN_TEST("SizeTest1", 0 == PQSize(pq1));
	PQEnqueue(pq1, &data1);
	RUN_TEST("SizeTest2", 1 == PQSize(pq1));
	PQEnqueue(pq1, &data2);
	RUN_TEST("SizeTest3", 2 == PQSize(pq1));
	PQEnqueue(pq1, &data3);

	RUN_TEST("SizeTest4", 3 == PQSize(pq1));

	PQDequeue(pq1);
	RUN_TEST("SizeTest5", 2 == PQSize(pq1));
	PQDequeue(pq1);
	RUN_TEST("SizeTest6", 1 == PQSize(pq1));
	PQDequeue(pq1);
	RUN_TEST("SizeTest7", 0 == PQSize(pq1));

	PQDestroy(pq1);
	
	return 0;
}

int ClearTest()
{
	int data1 = 7;
	int data2 = 5;
	int data3 = 3;	

	p_queue_t *pq1 = PQCreate(NULL, CompareFuncInt);

	PQEnqueue(pq1, &data1);
	PQEnqueue(pq1, &data2);
	PQEnqueue(pq1, &data3);

	PQClear(pq1);

	RUN_TEST("ClearTest", 1 == PQIsEmpty(pq1));

	PQDestroy(pq1);
	
	return 0;
}

int EraseTest()
{
	int data1 = 7;
	int data2 = 3;
	int data3 = 5;

	int data_to_delete1 = 10;
	int data_to_delete2 = 3;

	p_queue_t *pq1 = PQCreate(NULL, CompareFuncInt);
	PQEnqueue(pq1, &data1);
	PQEnqueue(pq1, &data2);
	PQEnqueue(pq1, &data3);

	RUN_TEST("EraseTest1", NULL == PQErase(&data_to_delete1, pq1, IsMatch));
	RUN_TEST("EraseTest2", 3 == *(int *)PQErase(&data_to_delete2, pq1, IsMatch));
	RUN_TEST("EraseTest3", 5 == *(int *)PQPeek(pq1));

	PQDestroy(pq1);
	
	return 0;
}

/*User's function to determine whether is match and OK to delete*/
static int IsMatch(const void *new_data, const void *param)
{
	return (*(int *)new_data == *(int *)param);
}

/*User's function to compare int priorities(lower value - higher priority)*/
static int CompareFuncInt(const void *new_data, const void *src_data, void *param)
{
	UNUSE(param);
	
	if (*(int *)new_data > *(int *)src_data)
	{
		return (-1);
	}

	else if (*(int *)src_data > *(int *)new_data)
	{
		return (1);
	}

	return 0; /*src_data == new_data*/
}
