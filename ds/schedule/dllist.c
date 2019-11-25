/**********************************************************************
 *	This code is representing my functions of DS_DLList RC2           *
 *	Lab: OL-79                                		                  *
 *	Author: Yonatan Vologdin 										  *
 *	Reviewer: Dvir Breitbart 	  								      *
 *	Date: 13.11.19													  *
 **********************************************************************/
#include <stdlib.h>	/*malloc*/
#include <assert.h>	/*assert*/

#include "dllist.h"	/*doubly linked list data structure*/

#define DEADBEEF ((void *)(0xDEADBEEF))

struct dl_list
{
	dll_node_t *begin;
	dll_node_t *end;
};

struct node 
{
	void *data;
	struct node *prev;
	struct node *next;
};

/*auxiliary function*/
static dll_node_t *CreateNode(void *data, dll_node_t *prev_node_ptr,
										  dll_node_t *next_node_ptr)
{
	dll_node_t *new_node = (dll_node_t *)malloc(sizeof(dll_node_t));
	if (NULL == new_node)
	{
		return NULL;
	}
	new_node->data = data;
	new_node->prev = prev_node_ptr;
	new_node->next = next_node_ptr;

	return new_node;
}

dl_list_t *DLListCreate(void)
{
	dll_node_t *begin_dummy = NULL;
	dll_node_t *end_dummy = NULL;
	dl_list_t *new_list = NULL;

	begin_dummy = CreateNode(DEADBEEF, NULL, NULL);
	if (NULL == begin_dummy)
	{
		return NULL;
	}

	end_dummy = CreateNode(DEADBEEF, begin_dummy, NULL);
	if (NULL == end_dummy)
	{
		free(begin_dummy);

		return NULL;
	}

	begin_dummy->next = end_dummy;
	
	new_list = (dl_list_t *)malloc(sizeof(dl_list_t));
	if (NULL == new_list)
	{
		free(begin_dummy);
		free(end_dummy);

		return NULL;
	}

	new_list->begin = begin_dummy;
	new_list->end = end_dummy;

	return new_list;
}

void DLListDestroy(dl_list_t *list)
{
	assert(list);

	list->end = list->begin;

	while (list->end->next)
	{
		list->begin = list->end;
		list->end = list->end->next;
		free(list->begin);
	}

	free(list->end);
	free(list);
}

dll_iter_t DLListPushBack(dl_list_t *list, void *data)
{
	assert(data);
	assert(list);	

	return DLListInsert(data, DLListEnd(list), list);
}

dll_iter_t DLListPushFront(dl_list_t *list, void *data)
{
	assert(data);
	assert(list);	

	return DLListInsert(data, DLListBegin(list), list);
}

dll_iter_t DLListInsert(void *data, dll_iter_t iterator, dl_list_t *list)
{
	dll_node_t *new_node = NULL;

	assert(data);
	assert(iterator);
	assert(list);	

	new_node = CreateNode(data, DLListPrev(iterator), iterator);
	if(NULL == new_node)
	{
		return list->end;
	}

	DLListPrev(iterator)->next = new_node;
	iterator->prev = new_node;

	return new_node;
}

void *DLListPopFront(dl_list_t *list)
{
	void *data = NULL;

	assert(list);	

	data = DLListBegin(list)->data;

	DLListRemove(DLListBegin(list));
	
	return data;
}

void *DLListPopBack(dl_list_t *list)
{
	void *data = NULL;

	assert(list);	

	data = DLListEnd(list)->prev->data;

	DLListRemove(DLListEnd(list)->prev);
	
	return data;
}

dll_iter_t DLListRemove(dll_iter_t iterator)
{
	dll_iter_t prev_node = NULL;
	dll_iter_t next_node = NULL;

	assert(iterator);	

	prev_node = DLListPrev(iterator);
	next_node = DLListNext(iterator);

	prev_node->next = next_node;
	next_node->prev = prev_node;

	free(iterator);

	return next_node;
}

dll_iter_t DLListSplice(dll_iter_t s_begin, dll_iter_t s_end, dll_iter_t dest)
{
	assert(s_begin);
	assert(s_end);
	assert(dest);

	/*excluding s_end*/
	s_end = s_end->prev;

	/*reconstructing src list*/
	s_begin->prev->next = s_end->next;
	s_end->next->prev = s_begin->prev;

	s_end->next = dest->next;
	dest->next->prev = s_end;

	dest->next = s_begin;
	s_begin->prev = dest;

	return dest;
}

size_t DLListSize(const dl_list_t *list)
{
	size_t count = 0;
	dll_iter_t run_iter = NULL;

	assert(list);

	for (run_iter = DLListBegin(list); run_iter != DLListEnd(list); ++count)
	{
		run_iter = DLListNext(run_iter);
	}

	return count;
}

int DLListIsEmpty(const dl_list_t *list)
{
	assert(list);

	return (DLListBegin(list) == DLListEnd(list));
}

dll_iter_t DLListFind(dll_iter_t begin, dll_iter_t end, 
					  const void *param, find_ptr ptr)
{
	assert(begin);
	assert(end);
	assert(param);

	/*goes from begin(including) to end(excluding)*/
	while (begin != end)
	{
		if (ptr(begin->data, param))
		{
			return begin;
		}
		begin = begin->next;
	}
	
	return end;
}

int DLListForEach(dll_iter_t begin, dll_iter_t end, 
				 	 void *param, for_each_ptr ptr)
{
	assert(begin);
	assert(end);
	assert(param);
	
	/*goes from begin(including) to end(excluding)*/
	while (begin != end)
	{
		if (ptr(begin->data, param))
		{	
			return 1;
		}
		begin = begin->next;
	}

	return 0;
}

dll_iter_t DLListBegin(const dl_list_t *list)
{
	assert(list);
		
	return list->begin->next;	
}

dll_iter_t DLListEnd(const dl_list_t *list)
{
	assert(list);
		
	return list->end;		
}

dll_iter_t DLListNext(dll_iter_t iterator)
{
	assert(iterator);
	assert(iterator->next);
		
	return iterator->next;
}

dll_iter_t DLListPrev(dll_iter_t iterator)
{
	assert(iterator);
	assert(iterator->prev);
		
	return iterator->prev;
}

void *DLListGetData(dll_iter_t iterator)
{
	assert(iterator);
		
	return iterator->data;	
}

int DLListIsSameIterator(dll_iter_t iterator1, dll_iter_t iterator2)
{
	assert(iterator1);
	assert(iterator2);

	return (iterator1 == iterator2);
}
