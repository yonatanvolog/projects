/**********************************************************************
 *	This code is representing my functions of Heap RC2		          *
 *	Lab: OL-79                                		                  *
 *	Author: Yonatan Vologdin 										  *
 *	Reviewer: Chen Rubin	 	  							     	  *
 *	Date: 31.12.19													  *
 **********************************************************************/
#include <stdlib.h>	/*malloc, free*/
#include <assert.h>	/*assert*/
#include <string.h> /*memcpy*/
#include <alloca.h>	/*alloca*/

#include "vector.h"
#include "heap.h"
#include "heapify.h"

#define VECTOR_CAPACITY (10)
#define POINTER_SIZE    (sizeof(void *))
#define FIRST_ELEMENT_INDEX 0

struct heap
{
	d_vector_t *vector;
	comparison_t comparison_func;
	void *param;
};

static void SwapIMP(void *new_data_ptr, void *parent_ptr, size_t elem_size);

static int GenericComparisonFuncIMP(const void *new_data, 
									const void *src_data,
									void *compare_param);
static void RemoveElementIMP(heap_t *heap, size_t element_to_remove_index, 
														  size_t elem_size);
static int FindElementIndexIMP(d_vector_t *vector, 
							   is_match_t is_match_func, 
							   void *param, size_t *data_index);

heap_t *HeapCreate(comparison_t comparison_func, void *comparison_param)
{
	heap_t *new_heap = NULL;
   	
	assert(comparison_func);
		
	new_heap = (heap_t *)malloc(sizeof(heap_t));
    if (NULL == new_heap)
    {
        return NULL;
    }

    new_heap->vector = VectorCreate(VECTOR_CAPACITY, POINTER_SIZE);
    if (NULL == new_heap->vector)
    {
        free(new_heap);
        
        return NULL;
    }
    new_heap->comparison_func = comparison_func;
    new_heap->param = comparison_param;
    
    return new_heap;
}

void HeapDestroy(heap_t *heap)
{
	assert(heap);

    VectorDestroy(heap->vector);
    free(heap);
}

int HeapPush(heap_t *heap, void *data)
{
    int res = 0;
    void *addr = NULL;
    size_t arr_size = 0;
	
	assert(heap);
	assert(data); 

    res = VectorPushBack(heap->vector, &data);
    if (0 != res)
    {
        return 1;
    }
    
    addr = VectorGetItemAddress(heap->vector, 0);
    arr_size = HeapSize(heap);
    
    HeapifyUp(addr, 
              arr_size,
              POINTER_SIZE,
              arr_size - 1,
              GenericComparisonFuncIMP,
              heap);
    
    return 0;
}

static int GenericComparisonFuncIMP(const void *new_data, 
									const void *src_data,
									void *compare_param)	
{
	heap_t *heap = compare_param;
	
	return (1 == heap->comparison_func(*(void **)new_data, 
								       *(void **)src_data, heap->param));
}

void HeapPop(heap_t *heap)
{
    void *first_address = NULL;
    void *last_address = NULL;
    size_t arr_size = 0;
    
	assert(heap);

    arr_size = HeapSize(heap) - 1;
	first_address = VectorGetItemAddress(heap->vector, 0);
	last_address = (char *)first_address + arr_size * POINTER_SIZE;
	
    SwapIMP(last_address, first_address, POINTER_SIZE);
    
    VectorPopBack(heap->vector);
    arr_size = HeapSize(heap);
    
    HeapifyDown(first_address, 
                arr_size,
                POINTER_SIZE,
                FIRST_ELEMENT_INDEX,
                GenericComparisonFuncIMP,
                heap);
}

static void SwapIMP(void *data1, void *data2, size_t elem_size)
{    
    void *tmp = alloca(elem_size);

    memcpy(tmp, data1, elem_size);
    memcpy(data1, data2, elem_size);
    memcpy(data2, tmp, elem_size);
}

void *HeapPeek(const heap_t *heap)
{   
	assert(heap);
	
    if (HeapIsEmpty(heap))
    {
        return NULL;
    }

    return *(void **)(VectorGetItemAddress(heap->vector, 0));
}

void *HeapRemove(heap_t *heap, is_match_t is_match_func, void *param)
{
	int res = 0;
	size_t element_to_remove_index = 0;
	void *removed_data = NULL;

	assert(heap);
	assert(is_match_func);
	
	res = FindElementIndexIMP(heap->vector, 
							  is_match_func, param, 
							  &element_to_remove_index);
	if (0 != res)
	{
		return NULL;
	}
	removed_data = *(void **)(VectorGetItemAddress(heap->vector, 
											       element_to_remove_index));
	RemoveElementIMP(heap, element_to_remove_index, POINTER_SIZE);

	HeapifyUp(VectorGetItemAddress(heap->vector, 0), 
              HeapSize(heap),
              POINTER_SIZE,
              element_to_remove_index,
              GenericComparisonFuncIMP,
              heap);	
	HeapifyDown(VectorGetItemAddress(heap->vector, 0), 
                HeapSize(heap),
                POINTER_SIZE,
                element_to_remove_index,
                GenericComparisonFuncIMP,
                heap);

	return removed_data;
}

static int FindElementIndexIMP(d_vector_t *vector, 
							   is_match_t is_match_func, 
							   void *param, size_t *data_index)
{
	size_t index = 0;
	size_t arr_size = VectorSize(vector);
	
	for (index = 0; index < arr_size; ++index)
	{
		if (1 == is_match_func(*(void **)VectorGetItemAddress(vector, index), 
																      param))
		{
			*data_index = index;

			return 0;
		}
	}
	
	return 1;
}

static void RemoveElementIMP(heap_t *heap, size_t element_to_remove_index, 
														  size_t elem_size)
{
	char *addr = VectorGetItemAddress(heap->vector, 0);
	memcpy(addr + element_to_remove_index*elem_size, 
		   VectorGetItemAddress(heap->vector, (HeapSize(heap) - 1)), elem_size);
	VectorPopBack(heap->vector);
}

int HeapIsEmpty(const heap_t *heap)
{
	assert(heap);
	
    return (0 == HeapSize(heap));
}

size_t HeapSize(const heap_t *heap)
{
	assert(heap);

    return (VectorSize(heap->vector));
}
