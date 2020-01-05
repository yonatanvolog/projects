/**********************************************************************
 *	This code is representing my functions of sorting algorithms RC2  *
 *														 			  *
 *	Includes the following functions: 								  *
 *	1. BubbleSort 													  *
 *	2. InsertionSort 												  *
 *	3. SelectionSort 												  *
 * 																	  *
 *	Lab: OL-79                                		                  *
 *	Author: Yonatan Vologdin 										  *
 *	Reviewer: Maoz Roytman	 	  								 	  *
 *	Date: 11.12.19													  *
 **********************************************************************/
#include <stdlib.h>		/*size_t*/
#include <sys/types.h>	/*ssize_t*/
#include <string.h>		/*memcpy*/
#include <assert.h>		/*assert*/

#include "../sorting.h"
#include "../../../yoni_utils.h"

#define DECIMAL_BASE 10
#define RADIX_HISTOGRAM_OFFSET 9

static void SwapIntIMP(int *a, int *b);
static void MoveSortedElementsByOneIMP(int *arr, size_t start, size_t end);
/*returns the index at which the arr[i] should be*/
static size_t FindIndexToInsertIMP(int *arr, size_t i);
static void SetMinValueIndexInSubArrayIMP(int *arr, size_t size, 
									   size_t *current_min_index, size_t start);

void BubbleSort(int *arr, size_t size)
{
	size_t i = 0;
	size_t is_sorted = 0;
	
	assert(arr);
	assert(size);
	
	for (; (!is_sorted) && (0 < size - 1); --size)
	{
		is_sorted = 1;

		for (i = 0; i < size - 1; ++i)
		{
			if (arr[i] > arr[i + 1])
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
	size_t i = 0;
	size_t index_to_insert = 0;
	int smallest_element = 0;
	
	assert(arr);
	assert(size);
	
	for (i = 1; i < size; ++i)
	{
		smallest_element = arr[i];
		index_to_insert = FindIndexToInsertIMP(arr, i);
		MoveSortedElementsByOneIMP(arr, index_to_insert, i);
		arr[index_to_insert] = smallest_element;
	}
}

static void MoveSortedElementsByOneIMP(int *arr, size_t start, size_t end)
{
	size_t i = 0;
	
	for (i = end; start < i; --i)
	{
		arr[i] = arr[i - 1];
	}
}

static size_t FindIndexToInsertIMP(int *arr, size_t current)
{
	ssize_t runner = current - 1;
	
	while ((runner >= 0) && (arr[current] < arr[runner]))
	{
		--runner;
	}
	
	return (size_t)(runner + 1);
}

void SelectionSort(int *arr, size_t size)
{
	size_t i = 0;
	size_t min_index = 0;

	assert(arr);
	assert(size);

	for (i = 0; i < size; ++i)
	{
		min_index = i;
		SetMinValueIndexInSubArrayIMP(arr, size, &min_index, i);
		SwapIntIMP(&arr[i], &arr[min_index]);
	}
}

static void SetMinValueIndexInSubArrayIMP(int *arr, size_t size, 
									   size_t *current_min_index, size_t start)
{
	size_t i = 0;

	for (i = start; i < size; ++i)
	{
		if (arr[i] < arr[*current_min_index])
		{	
			*current_min_index = i;
		}
	}
}
