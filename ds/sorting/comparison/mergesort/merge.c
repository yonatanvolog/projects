/**********************************************************************
 *	This code is representing my merge sort RC1             	      *
 *	Lab: OL-79                                		                  *
 *	Author: Yonatan Vologdin 										  *
 *	Reviewer: Gal Salemon 	 	  						     	      *
 *	Date: 30.12.19													  *
 **********************************************************************/
#include <stdlib.h>	/*malloc, free*/
#include <assert.h>	/*assert*/
#include <string.h>	/*memcpy*/

#include "../../sorting.h"
#include "../../../../yoni_utils.h"

static int RecMergeSortIMP(int *arr, size_t start, size_t end);
static int MergeIMP(int *arr, size_t start, size_t middle, size_t end);
static void CopyInSortedWayToMainArrIMP(int *arr, size_t start,
										int *left_arr,  size_t left_arr_len,
									    int *right_arr, size_t right_arr_len);
static int AllocateTwoSubArraysIMP(int **left_arr, size_t left_arr_len,
								   int **right_arr, size_t right_arr_len);
static void CopyArrToSubArraysIMP(int *arr, size_t start, size_t middle,
							      int *left_arr,  size_t left_arr_len,
							      int *right_arr, size_t right_arr_len);

int MergeSort(int *arr, size_t arr_size)
{
	assert(arr);	

	return RecMergeSortIMP(arr, 0, arr_size);
}

static int RecMergeSortIMP(int *arr, size_t start, size_t end)
{
	size_t middle = start + (end - start)/2;

	if (start == end - 1)
	{
		return 0;
	}

	RecMergeSortIMP(arr, start, middle);
	RecMergeSortIMP(arr, middle, end);

	return MergeIMP(arr, start, middle, end);
}

/*merges 2 subarrays of arr, 
left is from start to middle, 
right is from middle to end*/
static int MergeIMP(int *arr, size_t start, size_t middle, size_t end)
{
	int *left_arr = NULL;
	int *right_arr = NULL;
	size_t left_arr_len = middle - start;
	size_t right_arr_len = end - middle;

	size_t res = AllocateTwoSubArraysIMP(&left_arr, left_arr_len,
					  			         &right_arr, right_arr_len);
	if (0 != res)
	{
		return 1;
	}

	CopyArrToSubArraysIMP(arr, start, middle,
					      left_arr,  left_arr_len,
					      right_arr, right_arr_len);

	CopyInSortedWayToMainArrIMP(arr, start,
						        left_arr, left_arr_len,
							    right_arr, right_arr_len);

	return 0;
}

static int AllocateTwoSubArraysIMP(int **left_arr, size_t left_arr_len,
								   int **right_arr, size_t right_arr_len)
{
	*left_arr = (int *)malloc(left_arr_len * sizeof(int));
	if (NULL == *left_arr)
	{
		return 1;
	}

	*right_arr = (int *)malloc(right_arr_len * sizeof(int));
	if (NULL == *right_arr)
	{
		return 1;
	}
	
	return 0;
}


static void CopyArrToSubArraysIMP(int *arr, size_t start, size_t middle,
							      int *left_arr,  size_t left_arr_len,
							      int *right_arr, size_t right_arr_len)
{
	size_t i = 0;

	for (i = 0; i < left_arr_len; ++i)
	{
        left_arr[i] = arr[start + i]; 
	}

	for (i = 0; i < right_arr_len; ++i) 
	{    
    	right_arr[i] = arr[middle + i]; 
	}
}

static void CopyInSortedWayToMainArrIMP(int *arr, size_t start,
									    int *left_arr,  size_t left_arr_len,
									    int *right_arr, size_t right_arr_len)
{
    size_t main_runner = start;
	size_t i = 0;
	size_t j = 0;
		
    while ((i < left_arr_len) && (j < right_arr_len)) 
    { 
        if (left_arr[i] < right_arr[j])
        {
            arr[main_runner] = left_arr[i]; 
            i++; 
        }

        else
        { 
            arr[main_runner] = right_arr[j]; 
            j++; 
        }
        main_runner++; 
    }

    while (i < left_arr_len) 
    { 
        arr[main_runner] = left_arr[i]; 
        main_runner++;
        i++;
    } 
  
    while (j < right_arr_len) 
    { 
        arr[main_runner] = right_arr[j];
        main_runner++;
        j++;
    }

	free(left_arr);
	free(right_arr);
}
