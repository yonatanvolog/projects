/**********************************************************************
 *	This code is representing my functions of DS_PQ RC2		          *
 *	Lab: OL-79                                		                  *
 *	Author: Yonatan Vologdin 										  *
 *	Reviewer: Dvir Breitbart 	  								 	  *
 *	Date: 19.11.19													  *
 **********************************************************************/
#include <stdlib.h>	/*malloc*/
#include <assert.h>	/*assert*/

#include "sortedlist.h"	/*sorted list data structure*/
#include "pq.h"			/*priority queue data structure*/

typedef struct sort_params_wrapper
{
	int(*compare_func)(const void *new_data, const void *src_data, void *param);
	void *priority_param;
}sort_params_t;

struct p_queue
{
	srt_list_t *srt_list;
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

	new_pq->srt_list = SrtListCreate(&(new_pq->sort_params), MyIsBefore);
	if (NULL == new_pq->srt_list)
	{
		free(new_pq);
	
		return NULL;
	}

	return new_pq;
}

int PQEnqueue(p_queue_t *p_queue, void *data)
{
	srt_iter_t new_node = {0};

	assert(p_queue);
	assert(data);

	new_node = SrtListInsert(data, p_queue->srt_list);

	return SrtListIsSameIterator(SrtListEnd(p_queue->srt_list), new_node);
}

void *PQDequeue(p_queue_t *p_queue)
{
	assert(p_queue);

	return SrtListPopFront(p_queue->srt_list);
}

void PQDestroy(p_queue_t *p_queue)
{
	assert(p_queue);
	
	SrtListDestroy(p_queue->srt_list);
	free(p_queue);
}

void *PQPeek(const p_queue_t *p_queue)
{
	assert(p_queue);

	return SrtListGetData(SrtListBegin(p_queue->srt_list));
}

int PQIsEmpty(const p_queue_t *p_queue)
{
	assert(p_queue);	
	
	return SrtListIsEmpty(p_queue->srt_list);
}

size_t PQSize(const p_queue_t *p_queue)
{
	assert(p_queue);

	return SrtListSize(p_queue->srt_list);
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
	srt_iter_t srt_iter = {0};
	srt_iter_t srt_list_begin = {0};
	srt_iter_t srt_list_end = {0};

	void *data_inside = NULL;

	assert(data);
	assert(p_queue);
	assert(is_match);
	
	srt_list_begin = SrtListBegin(p_queue->srt_list);
	srt_list_end = SrtListEnd(p_queue->srt_list);

	srt_iter = SrtListFindIf(srt_list_begin, srt_list_end, data, is_match);
			
	if (SrtListIsSameIterator(srt_iter, srt_list_end))
	{
		return NULL;
	}

	data_inside = SrtListGetData(srt_iter);
	SrtListRemove(srt_iter);

	return data_inside;
}
