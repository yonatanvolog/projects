/**********************************************************************
 *	This code is representing my functions of DS_SRTList RC4          *
 *	Lab: OL-79                                		                  *
 *	Author: Yonatan Vologdin 										  *
 *	Reviewer: Gal Salemon   	  								      *
 *	Date: 18.11.19													  *
 **********************************************************************/
#include <stdlib.h>	/*malloc*/
#include <assert.h>	/*assert*/

#include "dllist.h"		/*doubly linked list data structure*/
#include "sortedlist.h"	/*sorted list data structure*/

#define	UNUSE(var) ((void)(var))

static int FindUnwrapperIMP(const void *data, const void *param);
static int InsertUnwrapperIMP(const void *data, const void *param);

typedef struct find_wrapper
{
	is_before_t is_before;
	const void *data;
	void *is_before_param;
}find_arg_wrapper;

struct srt_list
{
	dl_list_t *dll;
	is_before_t is_before_ptr;
	void *is_before_param;
};

srt_list_t *SrtListCreate(void *param, is_before_t is_before)
{
	srt_list_t *new_list = (srt_list_t *)malloc(sizeof(srt_list_t));
	if (NULL == new_list)
	{
		return NULL;
	}

	new_list->dll = DLListCreate();
	if (NULL == new_list->dll)
	{
		free(new_list);
				
		return NULL;
	}
	new_list->is_before_ptr = is_before;
	new_list->is_before_param = param;

	return new_list;
}

void SrtListDestroy(srt_list_t *list)
{
	assert(list);

	DLListDestroy(list->dll);
	free(list);
}

srt_iter_t SrtListInsert(void *data, srt_list_t *list)
{
	find_arg_wrapper *wrap_struct  = NULL;
	find_ptr un_wrapper = NULL;
	srt_iter_t srt_return_iter = {NULL};

	assert(list);
	assert(data);

	wrap_struct = (find_arg_wrapper *)malloc(sizeof(find_arg_wrapper));
	if (NULL == wrap_struct)
	{
		return srt_return_iter;
	}

	wrap_struct->is_before = list->is_before_ptr;
	wrap_struct->is_before_param = list->is_before_param;
	wrap_struct->data = data;

	un_wrapper = InsertUnwrapperIMP;

	srt_return_iter.dll_iterator =
	DLListFind(DLListBegin(list->dll), DLListEnd(list->dll), wrap_struct, un_wrapper);

	srt_return_iter.dll_iterator = 
	DLListInsert(data, srt_return_iter.dll_iterator, list->dll);

	free(wrap_struct);

	return srt_return_iter;
}

static int InsertUnwrapperIMP(const void *data, const void *param)
{
	const find_arg_wrapper *wrap_struct = (find_arg_wrapper *)param;
	
	return wrap_struct->is_before(wrap_struct->data, data, wrap_struct->is_before_param);
}

void *SrtListPopFront(srt_list_t *list)
{	
	assert(list);

	return DLListPopFront(list->dll);
}

void *SrtListPopBack(srt_list_t *list)
{
	assert(list);

	return DLListPopBack(list->dll);
}

srt_iter_t SrtListRemove(srt_iter_t iterator)
{		
	srt_iter_t next_iter = {NULL};

	assert (iterator.dll_iterator);

	next_iter.dll_iterator = DLListRemove(iterator.dll_iterator);
	
	return next_iter; 
}

size_t SrtListSize(const srt_list_t *list)
{
	assert(list);	
	
	return DLListSize(list->dll);
}

int SrtListIsEmpty(const srt_list_t *list)
{	
	assert(list);

	return DLListIsEmpty(list->dll);
}

srt_iter_t SrtListFind(srt_iter_t begin, srt_iter_t end, 
					   const void *data, srt_list_t *list)
{
	find_arg_wrapper *wrap_struct  = NULL;
	find_ptr un_wrapper = NULL;
	srt_iter_t srt_return_iter = {NULL};

	assert(begin.dll_iterator);
	assert(end.dll_iterator);
	assert(list);

	wrap_struct = (find_arg_wrapper *)malloc(sizeof(find_arg_wrapper));
	if (NULL == wrap_struct)
	{
		return srt_return_iter;
	}

	wrap_struct->is_before = list->is_before_ptr;
	wrap_struct->is_before_param = list->is_before_param;
	wrap_struct->data = data;

	un_wrapper = FindUnwrapperIMP;
	srt_return_iter.dll_iterator =
	DLListFind(begin.dll_iterator, end.dll_iterator, wrap_struct, un_wrapper);

	free(wrap_struct);

	return srt_return_iter;
}

static int FindUnwrapperIMP(const void *data, const void *param)
{
	int test1 = 0, test2 = 0;
	find_arg_wrapper *wrap_struct = (find_arg_wrapper *)param;
	
	test1 = wrap_struct->is_before(wrap_struct->data, data, (void *)param);
	test2 = wrap_struct->is_before(data, wrap_struct->data, (void *)param);

	if ((test1 == 1) && (test2 == 1))
	{
		return 1;
	}
	return 0;
}

srt_iter_t SrtListFindIf(srt_iter_t begin, srt_iter_t end, 
						 const void *param, is_match_func_t match_func)
{
	srt_iter_t srt_return_iter = {NULL};

	assert(begin.dll_iterator);
	assert(end.dll_iterator);
	assert(match_func);

	srt_return_iter.dll_iterator = 
	DLListFind(begin.dll_iterator, end.dll_iterator, param, match_func);

	return srt_return_iter; 
}

int SrtListForEach(srt_iter_t begin, srt_iter_t end, void *param,
										 action_func_t action_func)
{
	assert(begin.dll_iterator);
	assert(end.dll_iterator);
	assert(action_func);
		
	return DLListForEach(begin.dll_iterator, end.dll_iterator, param, action_func);
}

void SrtListMerge(srt_list_t *src_list, srt_list_t *dest_list)
{
	dll_iter_t dest_i 		= DLListBegin(dest_list->dll);
	dll_iter_t src_i_start	= DLListBegin(src_list->dll);
	dll_iter_t src_i_end	= DLListBegin(src_list->dll);

	for(; 0 == DLListIsSameIterator(dest_i, DLListEnd(dest_list->dll)) 
		; dest_i = DLListNext(dest_i))
	{
		while (*(int *)DLListGetData(src_i_end) <= *(int *)DLListGetData(dest_i))
		{
			src_i_end = DLListNext(src_i_end);
		}
					
		if (0 == DLListIsSameIterator(src_i_end, src_i_start))
		{
			DLListSplice(DLListBegin(src_list->dll), src_i_end, DLListPrev(dest_i));
		}

		src_i_start = src_i_end;
	}
	DLListSplice(DLListBegin(src_list->dll), DLListEnd(src_list->dll), DLListPrev(dest_i));

	SrtListDestroy(src_list);
}

srt_iter_t SrtListBegin(srt_list_t *list)
{
	srt_iter_t return_iter = {NULL};
	
	assert(list);
	
	return_iter.dll_iterator = DLListBegin(list->dll);
	
	return return_iter; 
}

srt_iter_t SrtListEnd(srt_list_t *list)
{
	srt_iter_t return_iter = {NULL};

	assert(list);

	return_iter.dll_iterator = DLListEnd(list->dll);
	
	return return_iter; 
}

srt_iter_t SrtListNext(srt_iter_t iterator)
{
	assert(iterator.dll_iterator);

	iterator.dll_iterator = DLListNext(iterator.dll_iterator);

	return iterator;
}

srt_iter_t SrtListPrev(srt_iter_t iterator)
{
	assert(iterator.dll_iterator);

	iterator.dll_iterator = DLListPrev(iterator.dll_iterator);

	return iterator;
}

void *SrtListGetData(srt_iter_t iterator)
{
	return DLListGetData(iterator.dll_iterator);
}

int SrtListIsSameIterator(srt_iter_t iterator1, srt_iter_t iterator2)
{
	return (iterator1.dll_iterator == iterator2.dll_iterator);
}
