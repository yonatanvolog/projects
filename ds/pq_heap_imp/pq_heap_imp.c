/**********************************************************************
 *	This code is representing my functions of Priority Queue          *
 *			                                		                  *
 *			        ***Based on Heap Data Structure*** 		 	      *
 *			                                		                  *
 *	Lab: OL-79                                		                  *
 *	Author: Yonatan Vologdin 										  *
 *	Date: 03.01.20													  *
 **********************************************************************/
#include <stdlib.h>	/*malloc*/
#include <assert.h>	/*assert*/

#include "heap.h"	/*heap data structure*/
#include "pq.h"		/*priority queue data structure*/

typedef struct sort_params_wrapper
{
	int(*compare_func)(const void *new_data, const void *src_data, void *param);
	void *priority_param;
}sort_params_t;

struct p_queue
{
	heap_t *heap;
	sort_params_t sort_params;
};

/*Wrapping function for user's compare_func, returns 0 or 1*/
static int MyIsBefore(const void *new_data, const void *src_data, void *param)
{
	int compare_func_res = 0;
	sort_params_t pq_wrapper = {0};

	assert(param);
	
	pq_wrapper = *(sort_params_t *)param;
	compare_func_res = 
	pq_wrapper.compare_func(new_data, src_data, pq_wrapper.priority_param);

	if ((1) == compare_func_res)
	{		
		return 1;
	}

	return 0;
}

p_queue_t *PQCreate(void *priority_param, 
int(*compare_func)(const void *new_data, const void *src_data, void *param))
{
	p_queue_t *new_pq = NULL;

	assert(compare_func);

	new_pq = (p_queue_t *)malloc(sizeof(p_queue_t));
	if (NULL == new_pq)
	{
		return new_pq;
	}

	new_pq->sort_params.compare_func = compare_func;
	new_pq->sort_params.priority_param = priority_param;

	new_pq->heap = HeapCreate(MyIsBefore, &(new_pq->sort_params));
	if (NULL == new_pq->heap)
	{
		free(new_pq);
	
		return NULL;
	}

	return new_pq;
}

int PQEnqueue(p_queue_t *p_queue, void *data)
{
	assert(p_queue);
	assert(data);

	return HeapPush(p_queue->heap, data);
}

void *PQDequeue(p_queue_t *p_queue)
{
	void *removed_data = HeapPeek(p_queue->heap);	
	
	assert(p_queue);

	HeapPop(p_queue->heap);

	return removed_data;
}

void PQDestroy(p_queue_t *p_queue)
{
	assert(p_queue);
	
	HeapDestroy(p_queue->heap);
	free(p_queue);
}

void *PQPeek(const p_queue_t *p_queue)
{
	assert(p_queue);

	return HeapPeek(p_queue->heap);
}

int PQIsEmpty(const p_queue_t *p_queue)
{
	assert(p_queue);	
	
	return HeapIsEmpty(p_queue->heap);
}

size_t PQSize(const p_queue_t *p_queue)
{
	assert(p_queue);

	return HeapSize(p_queue->heap);
}

void PQClear(p_queue_t *p_queue)
{
	assert(p_queue);

	while(!PQIsEmpty(p_queue))
	{
		PQDequeue(p_queue);
	}
}

void *PQErase(void *data, p_queue_t *p_queue, 
 			  int(*is_match)(const void *new_data, const void *param))
{
	assert(p_queue);
	assert(data);
	assert(is_match);
	
	return HeapRemove(p_queue->heap, is_match, data);
}

