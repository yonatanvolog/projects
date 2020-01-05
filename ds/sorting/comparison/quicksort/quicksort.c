/**********************************************************************
 *	This code is representing my QuickSort RC2              	      *
 *	Lab: OL-79                                		                  *
 *	Author: Yonatan Vologdin 										  *
 *	Reviewer: Gal Salemon 	 	  						     	      *
 *	Date: 04.01.20													  *
 **********************************************************************/
#include <stdlib.h>	/*malloc, free*/
#include <assert.h>	/*assert*/
#include <string.h>	/*memcpy*/
#include <alloca.h> /*alloca*/

#include "../../sorting.h"
#include "../../../../yoni_utils.h"

static void SwapIMP(void *data1, void *data2, size_t size);
static void QuickSortIMP(void *, int left, int right,
						 size_t elem_size, compar_func_t func);
static int PartitionIMP(void *, int left, int right, 
						size_t elem_size, compar_func_t func);

void QuickSort(void *arr, size_t arr_size, size_t elem_size, compar_func_t func)
{
	QuickSortIMP(arr, 0, arr_size -1, elem_size, func);
}

static void QuickSortIMP(void *arr, int left, int right, 
				  size_t elem_size, compar_func_t func)
{
	int index = PartitionIMP(arr, left, right, elem_size, func);

	if (left < right)
	{
		QuickSortIMP(arr, left, index -1, elem_size, func);
		QuickSortIMP(arr, index, right, elem_size, func);
	}
}

static int PartitionIMP(void *arr, int left, int right, 
				 size_t elem_size, compar_func_t func)
{
    void *pivot = (void *)alloca(elem_size);
	memcpy(pivot, ((char *)arr + ((left + right) / 2) * elem_size), elem_size);

	while (left <= right)
	{
		while (0 < func(pivot, (char *)arr + left * elem_size))
		{
			++left;
		}

		while (0 < func((char *)arr + right * elem_size, pivot))
		{
 			--right;
		}

		if (left <= right)
		{
			SwapIMP((char *)arr + left * elem_size, 
					(char *)arr + right * elem_size, elem_size);
			++left;
			--right;
		}
	}

	return left;
}

static void SwapIMP(void *data1, void *data2, size_t elem_size)
{
    void *tmp = (void *)alloca(elem_size);
    memcpy(tmp, data1, elem_size);
    memcpy(data1, data2, elem_size);
    memcpy(data2, tmp, elem_size);
}
