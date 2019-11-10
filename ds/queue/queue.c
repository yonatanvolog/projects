/**********************************************************************
 *	This code is representing my functions of DS_Queue RC2            *
 *	Lab: OL-79                                		                  *
 *	Author: Yonatan Vologdin 										  *
 *	Reviewer: Eyal Rabinovitz    								      *
 *	Date: 10.11.19													  *
 **********************************************************************/
#include <stdlib.h>	/*malloc*/
#include <assert.h>	/*assert*/

#include "slist.h"	/*singly linked list structure*/
#include "queue.h"	/*queue structure*/

#define DUMMY 1

struct queue
{
    sl_node_t *rear;
    sl_node_t *front;
};

queue_t *QCreate()
{
	int initial_data = 888;
	queue_t *dummy = (queue_t *)malloc(sizeof(queue_t));
	
	if (NULL == dummy)
	{	
		return NULL;
	}
	dummy->rear = SListCreateNode(&initial_data, NULL); 
	if (NULL == dummy)
	{	
		free(dummy);
		
		return NULL;
	}
	(dummy->rear)->data = &initial_data;
	dummy->front = dummy->rear;

	return dummy;
}

int QEnqueue(queue_t *queue, void *data)
{
	sl_node_t* node_to_enqueue = NULL;

	assert(queue);
	assert(data);

	node_to_enqueue = SListCreateNode(data, NULL);
	if(NULL == node_to_enqueue)
	{
		return 1;
	}
	SListInsert(node_to_enqueue, queue->rear);
	queue->rear = queue->rear->next;
	
	return 0;
}

void QDequeue(queue_t *queue)
{
	sl_node_t* node_to_dequeue = NULL;

	assert(queue);

	node_to_dequeue= queue->front;
	queue->front = queue->front->next;
	free(node_to_dequeue);
}

void *QPeek(const queue_t *queue)
{
	assert(queue);

	return ((queue->front)->data);
}

int QIsEmpty(const queue_t *queue)
{
	assert(queue);

	return (queue->front == queue->rear);
}

void QDestroy(queue_t *queue)
{	
	assert(queue);

	SListFreeAll(queue->front);
	queue->front = NULL;
	queue->rear = NULL;
	free(queue);
}

size_t QSize(const queue_t *queue)
{
	assert(queue);

	return (SListCount(queue->front) - DUMMY);
}

void QAppend(queue_t *dest, queue_t *src)
{
	sl_node_t* node_to_remove = NULL;

	assert(dest);
	assert(src);

	node_to_remove = src->front;
	dest->rear->next = src->front;
	SListRemove(dest -> rear);
	dest->rear = src -> rear;

	free(src);
	free(node_to_remove);
}

