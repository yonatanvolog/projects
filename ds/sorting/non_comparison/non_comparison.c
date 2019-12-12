/**********************************************************************
 *	This code is representing my functions of sorting algorithms RC2  *
 *														 			  *
 *	Includes the following functions: 								  *
 *	1. CountingSortIMP 												  *
 *	2. CountingSort                                                   *
 *	3. RadixSort  													  *
 * 																	  *
 *	Lab: OL-79                                		                  *
 *	Author: Yonatan Vologdin 										  *
 *	Reviewer: Maoz Roytman	 	  								 	  *
 *	Date: 12.12.19													  *
 **********************************************************************/
#include <stdlib.h>		/*size_t*/
#include <sys/types.h>	/*ssize_t*/
#include <string.h>		/*memcpy*/
#include <assert.h>		/*assert*/

#include "../sorting.h"
#include "yoni_utils.h"

#define DECIMAL_BASE 10
#define RADIX_HISTOGRAM_OFFSET 9

static size_t CountingGetBucketIMP(void *element, void *param);
static void	PopulateHistogramCountIMP(const void **arr_to_sort, size_t arr_size,
								   size_t element_size,
								   unsigned int *histogram,	
					 size_t (*CountingGetBucketIMP)(void *element, void *param),
						 		   void *param);
static void	SumUpHistogramIMP(unsigned int *histogram, size_t his_size);

static void SortElementsInResultArrayIMP(const void **arr_to_sort, size_t arr_size,
								   size_t element_size,
								   unsigned int *histogram,	
					 size_t (*CountingGetBucketIMP)(void *element, void *param),
								   void **result, void *param);
static size_t RadixGetBucketIMP(void *element, void *param);
static int GetAbsMaxElementIMP(int *arr, size_t arr_size);
static size_t GeNumOfDigitsIMP(int num);

static size_t CountingGetBucketIMP(void *element, void *param)
{
    return ((*(int *)element) - (*(int *)param));
}

static void	PopulateHistogramCountIMP(const void **arr_to_sort, size_t arr_size,
								   size_t element_size,
								   unsigned int *histogram,	
					 size_t (*CountingGetBucketIMP)(void *element, void *param),
						 		   void *param)
{	
	size_t i = 0;
	size_t index_in_histogram = 0;

	for (i = 0; i < arr_size; ++i)
	{
		index_in_histogram = CountingGetBucketIMP((char *)(* arr_to_sort) +
												 (i * element_size), param);
		*(histogram + index_in_histogram) += 1;
	}
}

static void	SumUpHistogramIMP(unsigned int *histogram, size_t his_size)
{	
	size_t i = 0;

	for (i = 1; i < his_size; ++i)
	{
		histogram[i] += histogram[i - 1];
	}
}

static void SortElementsInResultArrayIMP(const void **arr_to_sort, size_t arr_size,
								   size_t element_size,
								   unsigned int *histogram,	
					 size_t (*CountingGetBucketIMP)(void *element, void *param),
								   void **result, void *param)
{
	size_t i = 0;
	size_t index_in_histogram = 0;
	size_t arr_to_sort_i = 0;

	for (i = arr_size - 1; (int)i >= 0; --i)
	{
		index_in_histogram = CountingGetBucketIMP((char *)(*arr_to_sort) + 
												 (i * element_size), param);
	
 		*(histogram + index_in_histogram) -= 1;
        arr_to_sort_i = *(histogram + index_in_histogram);
		memcpy((char *)(*result) 	   + (arr_to_sort_i * element_size),
               (char *)(*arr_to_sort) + (i * element_size),
			 	element_size); 
	}
}

void CountingSortIMP(const void *arr_to_sort, size_t element_size, size_t arr_size,
					 unsigned int *histogram, size_t his_size,
					 size_t (*CountingGetBucketIMP)(void *element, void *param),
					 void *param,
					 void *result)
{
	assert(arr_to_sort);
	assert(element_size);
	assert(arr_size);
	assert(histogram);
	assert(his_size);
	assert(CountingGetBucketIMP);
	assert(result);


	PopulateHistogramCountIMP(&arr_to_sort, arr_size, element_size,
						   histogram, CountingGetBucketIMP, param);
	
	SumUpHistogramIMP(histogram, his_size);
	
	SortElementsInResultArrayIMP(&arr_to_sort, arr_size, element_size,
						   histogram, CountingGetBucketIMP, &result, param);
}

int CountingSort(const int *arr, size_t arr_size,
				 int min_val, int max_val,
				 int *result)
{	
	size_t histogram_size = (max_val - min_val) + 1;
	unsigned int *histogram = NULL;

	assert(arr);
	assert(arr_size);
	assert(result);

	histogram = (unsigned int *)calloc(histogram_size, sizeof(unsigned int));
	if (NULL == histogram)
	{
		return 1;
	}	

	CountingSortIMP(arr, sizeof(int), arr_size,
					histogram, histogram_size,
					CountingGetBucketIMP, &min_val,
					result);

	free(histogram);

	return 0;
}

int RadixSort(int *arr, size_t arr_size)
{
	size_t i = 0;
	size_t histogram_size = ABS(-9) + 1 + 9;
	size_t max_digit_count = 0;
	int digit_position_mask = DECIMAL_BASE;
	int *result = NULL;
	unsigned int *histogram = NULL;

	assert(arr);
	assert(arr_size);

	result = (int *)malloc(arr_size * sizeof(int));
	if (NULL == result)
	{
		return 1;
	}

	histogram = (unsigned int *)calloc(histogram_size, sizeof(unsigned int));
	if (NULL == histogram)
	{
		free(result);
		
		return 1;
	}

	max_digit_count = GeNumOfDigitsIMP(GetAbsMaxElementIMP(arr, arr_size));

	for (i = 0; i < max_digit_count; ++i)
	{
		CountingSortIMP(arr, sizeof(int), arr_size,
						histogram, histogram_size,
						RadixGetBucketIMP, &digit_position_mask,
						result);
		
		memset(histogram, 0, (histogram_size * sizeof(unsigned int)));
		memcpy(arr, result, (arr_size * sizeof(int)));
		digit_position_mask *= DECIMAL_BASE;	
	}

	free(result);
	free(histogram);
	
	return 0;
}

static size_t RadixGetBucketIMP(void *element, void *param)
{	
	int num = (*(int *)element);
	int set_num_back = (*(int *)param) / DECIMAL_BASE;

    return (((num / set_num_back) % DECIMAL_BASE) + RADIX_HISTOGRAM_OFFSET);
}

static int GetAbsMaxElementIMP(int *arr, size_t arr_size)
{
	int max_element = arr[0];
	size_t i = 0;

	for (i = 0; i < arr_size; ++i)
	{
		max_element = MAX(max_element, ABS(arr[i]));
	}
	
	return max_element;
}

static size_t GeNumOfDigitsIMP(int num)
{
	size_t num_of_digits = 0;

	while (0 < num)
	{
		++num_of_digits;
		num /= DECIMAL_BASE;
	}

	return num_of_digits;
}
