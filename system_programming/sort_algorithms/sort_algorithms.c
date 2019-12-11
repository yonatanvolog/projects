/**********************************************************************
 *	This code is representing my functions of sorting algorithms RC1  *
 *	Lab: OL-79                                		                  *
 *	Author: Yonatan Vologdin 										  *
 *	Reviewer: Maoz Roytman	 	  								 	  *
 *	Date: 10.12.19													  *
 **********************************************************************/
#include <stdlib.h>		/*size_t*/
#include <sys/types.h>	/*ssize_t*/
#include <string.h>

#include "yoni_utils.h"	/*utils*/

static void SwapIntIMP(int *a, int *b);
/*returns the index at which the arr[i] should be*/
static size_t FindLocIMP(int *arr, size_t i);

void BubbleSort(int *arr, size_t size)
{
	size_t i = 0, is_sorted = 0;
		
	for(; (!is_sorted) && (0 < size - 1); --size)
	{
		is_sorted = 1;
		for(i = 0; i < size - 1; ++i)
		{
			if(arr[i] > arr[i + 1])
			{
				SwapIntIMP(&arr[i], &arr[i + 1]);
				is_sorted = 0;
			}
		}
	}
}

static void SwapIntIMP(int *a, int *b)
{
	int tmp = *a;
	*a = *b;
	*b = tmp;
}

void InsertionSort(int *arr, size_t size)
{
	size_t i = 0, j = 0, loc = 0;
	int num = 0;
	
	for(i = 1; i < size; ++i)
	{
		num = arr[i];
		loc = FindLocIMP(arr, i);
		
		for(j = i; loc < j; --j)
		{
			arr[j] = arr[j-1];
		}
		
		arr[loc] = num;
	}
}

static size_t FindLocIMP(int *arr, size_t i)
{
	ssize_t k = i - 1;
	
	while((k >= 0) && (arr[i] < arr[k]))
	{
		--k;
	}
	
	return (size_t)(k + 1);
}

void SelectionSort(int *arr, size_t size)
{
	size_t i = 0, j = 0, min_index = 0;

	for(i = 0; i < size; ++i)
	{
		min_index = i;

		for(j = i; j < size; ++j)
		{
			if(arr[j] < arr[min_index])
			{	
				min_index = j;
			}
		}
		SwapIntIMP(&arr[i], &arr[min_index]);
	}
}

size_t GetBucketIMP(void *element, void *param)
{
    return ((*(int *)element) - (*(int *)param));
}

void CountingSortIMP(const void *arr_to_sort,
					 size_t element_size,
					 size_t arr_size,
					 unsigned int *histogram,
					 size_t his_size,
					 size_t (*GetBucketIMP)(void *element, void *param),
					 void *param,
					 void *result)
{
	size_t i = 0;
	size_t index_to_increment = 0;
	size_t index_in_histogram = 0;
	size_t arr_to_sort_i = 0;

	/*populate histogram count*/
	for(i = 0; i < arr_size; ++i)
	{
		index_to_increment = GetBucketIMP((char *)arr_to_sort + (i * element_size), param);
		++histogram[index_to_increment];
	}

	/*sum up histogram*/
	for(i = 1; i < his_size; ++i)
	{
		histogram[i] += histogram[i - 1];
	}

	/*sort elements in result array*/
    for (i = arr_size - 1; (int)i >= 0; --i)
	{
		index_in_histogram = GetBucketIMP((char *)arr_to_sort + (i * element_size), param);
	
 		--histogram[index_in_histogram];
        arr_to_sort_i = histogram[index_in_histogram];
		memcpy((char *)result 	   + (arr_to_sort_i * element_size),
               (char *)arr_to_sort + (i * element_size),
			 	element_size); 
	}
}

int CountingSort(const int *arr,
				 size_t arr_size,
				 int min_val,
				 int max_val,
				 int *result)
{	
	size_t his_size = (max_val - min_val) + 1;
	unsigned int *histogram = NULL;

	histogram = (unsigned int *)calloc(his_size, sizeof(unsigned int));
	if(NULL == histogram)
	{
		return 1;
	}	

	CountingSortIMP(arr,
					sizeof(int),
					arr_size,
					histogram,
					his_size,
					GetBucketIMP,
					&min_val,
					result);

	free(histogram);	

	return 0;
}
























