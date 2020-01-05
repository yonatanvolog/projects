/**********************************************************************
 *	This code is representing my implementation of Heap Sort RC2      *
 *	Lab: OL-79                                		                  *
 *	Author: Yonatan Vologdin 										  *
 *	Reviewer: Chen Rubin 	 	  				    		     	  *
 *	Date: 03.01.20													  *
 **********************************************************************/
#include <alloca.h> /* alloca */
#include <stdlib.h> /* for size_t */
#include <assert.h> /* for assert */
#include <string.h> /* memcpy */

#include "heapify.h"
#include "../../sorting.h"


#define FIRST_INDEX 0

static int IsBeforeWrapperIMP(const void *new_data, 
			  	 			  const void *src_data, 
			   				  void *param);

typedef struct is_before_wrapper
{
	is_before_t func;
	void *is_before_param;
}wrapper_t;

static void GenericSwapIMP(void *data1, void *data2, size_t elem_size);

void HeapSort(int *arr, size_t arr_size, size_t elem_size, 
			  			     is_before_t func, void *param)
{
	int i = 0;

	wrapper_t my_wrapper = {0};
	my_wrapper.func = func;
	my_wrapper.is_before_param = param;

	for (i = (arr_size / 2) - 1; FIRST_INDEX <= i; --i)
	{
		HeapifyDown(arr, arr_size,
					elem_size, i,
					IsBeforeWrapperIMP, &my_wrapper);
	}

	for (i = arr_size - 1; FIRST_INDEX <= i; --i)
	{
		GenericSwapIMP(arr, arr + i, elem_size);

		HeapifyDown(arr, i, 
					elem_size, FIRST_INDEX,
				    IsBeforeWrapperIMP, &my_wrapper);
	}
}

static int IsBeforeWrapperIMP(const void *new_data, 
			  	 			  const void *src_data, 
			   				  void *param)
{
	wrapper_t *wrapper = (wrapper_t *)param;
	
	int res = wrapper->func(new_data, src_data, (wrapper->is_before_param));
	
	return (!res);
}	

static void GenericSwapIMP(void *data1, void *data2, size_t elem_size)
{
    void *tmp = alloca(elem_size);
     
    memmove(tmp, data1, elem_size);
    memmove(data1, data2, elem_size);
    memmove(data2, tmp, elem_size);
}
