/**********************************************************************
 *	This code is representing my tests of DS_Queue RC2                *
 *	Lab: OL-79                                		                  *
 *	Author: Yonatan Vologdin 										  *
 *	Reviewer: Eyal Rabinovitz    								      *
 *	Date: 10.11.19													  *
 **********************************************************************/
#include <stdio.h>	/*printf*/

#include "slist.h"	/*singly linked list structure*/
#include "queue.h"	/*queue structure*/

#define RESULT(condition) ((condition) ? 1 : 0)

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define RUN_TEST(test_name, result) \
(printf("%s %s\n", test_name, (1 == (result)) ? KGRN"\tPassed"KNRM : KRED"\tFailed"KNRM))

struct queue
{
    sl_node_t *rear;
    sl_node_t *front;
};

int CreateDestroyTest();
int EnqueueTest();
int DequeueIsEmptyTest();
int PeekTest();
int SizeTest();
int AppendTest();

int main()
{
	RUN_TEST("CreateDestroyTest:", CreateDestroyTest());
	RUN_TEST("EnqueueTest:\t", EnqueueTest());
	RUN_TEST("DequeueIsEmptyTest:", DequeueIsEmptyTest());
	RUN_TEST("PeekTest:\t", PeekTest());
	RUN_TEST("SizeTest:\t", SizeTest());
	RUN_TEST("AppendTest:\t", AppendTest());
	
	return 0;
}

int CreateDestroyTest()
{
	int result = 0;
	queue_t* super = QCreate();
	result = RESULT((super->front == super->rear));
	QDestroy(super);
	
	return result;
}

int EnqueueTest()
{
	int result = 0;
	int enqueue1_data = 1;
	int enqueue2_data = 2;
	int enqueue3_data = 3;

	queue_t* super = QCreate();
	
	QEnqueue(super, &enqueue1_data);
	result += RESULT(1 == *(int *)super->front->data);

	QEnqueue(super, &enqueue2_data);
	result += RESULT(2 == *(int *)super->front->next->data);

	QEnqueue(super, &enqueue3_data);
	result += RESULT(3 == *(int *)super->front->next->next->data);

	result = RESULT(3 == result);
	QDestroy(super);

	return result;
}

int DequeueIsEmptyTest()
{
	int result = 0;
	int enqueue1_data = 1;
	int enqueue2_data = 2;
	int enqueue3_data = 3;

	queue_t* super = QCreate();
	
	QEnqueue(super, &enqueue1_data);
	QEnqueue(super, &enqueue2_data);
	QEnqueue(super, &enqueue3_data);

	result += RESULT(0 == QIsEmpty(super));

	QDequeue(super);
	QDequeue(super);
	QDequeue(super);
	
	result += RESULT(1 == QIsEmpty(super));
	
	result = RESULT(2 == result);
	
	QDestroy(super);

	return result;
}

int PeekTest()
{
	int result = 0;
	int enqueue1_data = 1;
	int enqueue2_data = 2;
	int enqueue3_data = 3;

	queue_t* super = QCreate();
	
	QEnqueue(super, &enqueue1_data);
	result += RESULT(1 == *(int *)QPeek(super));
	QEnqueue(super, &enqueue2_data);
	result += RESULT(1 == *(int *)QPeek(super));
	QEnqueue(super, &enqueue3_data);
	result += RESULT(1 == *(int *)QPeek(super));

	QDequeue(super);
	result += RESULT(2 == *(int *)QPeek(super));
	QDequeue(super);
	result += RESULT(3 == *(int *)QPeek(super));
	QDequeue(super);
	
	result = RESULT(5 == result);
	
	QDestroy(super);

	return result;
}

int SizeTest()
{
	int result = 0;
	int enqueue1_data = 1;
	int enqueue2_data = 2;
	int enqueue3_data = 3;

	queue_t* super = QCreate();
	
	result += RESULT(0 == (int)QSize(super));
	QEnqueue(super, &enqueue1_data);
	result += RESULT(1 == (int)QSize(super));
	QEnqueue(super, &enqueue2_data);
	result += RESULT(2 == (int)QSize(super));
	QEnqueue(super, &enqueue3_data);
	result += RESULT(3 == (int)QSize(super));

	QDequeue(super);
	result += RESULT(2 == (int)QSize(super));
	QDequeue(super);
	result += RESULT(1 == (int)QSize(super));
	QDequeue(super);
	result += RESULT(0 == (int)QSize(super));
		
	result = RESULT(7 == result);
	
	QDestroy(super);

	return result;
}

int AppendTest()
{
	int result = 0;

	/*source*/
	int enqueue1_data = 1;
	int enqueue2_data = 2;

	/*dest*/
	int enqueue3_data = 3;
	int enqueue4_data = 4;
	int enqueue5_data = 5;
	int enqueue6_data = 6;

	queue_t* scr_super = QCreate();
	queue_t* dest_super = QCreate();

	QEnqueue(scr_super, &enqueue1_data);
	QEnqueue(scr_super, &enqueue2_data);

	QEnqueue(dest_super, &enqueue3_data);
	QEnqueue(dest_super, &enqueue4_data);
	QEnqueue(dest_super, &enqueue5_data);
	QEnqueue(dest_super, &enqueue6_data);

	QAppend(dest_super, scr_super);

	result += RESULT(6 == QSize(dest_super));

	QDequeue(dest_super);
	result += RESULT(4 == *(int *)QPeek(dest_super));
	QDequeue(dest_super);
	result += RESULT(5 == *(int *)QPeek(dest_super));
	QDequeue(dest_super);
	result += RESULT(6 == *(int *)QPeek(dest_super));
	QDequeue(dest_super);
	result += RESULT(1 == *(int *)QPeek(dest_super));
	QDequeue(dest_super);
	result += RESULT(2 == *(int *)QPeek(dest_super));

	result = RESULT(6 == result);
	
	QDestroy(dest_super);

	return result;
}
