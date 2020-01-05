/**********************************************************************
 *	This code is representing my tests of Heapify       	          *
 *	Lab: OL-79                                		                  *
 *	Author: Yonatan Vologdin 										  *
 *	Reviewer: Chen Rubin    				          			      *
 *	Date: 04.01.20													  *
 **********************************************************************/

#include <stdio.h>	/*printf*/
#include <string.h> /*memcmp*/

#include "heapify.h"
#include "../../yoni_utils.h" /*UNUSE*/

static void PrintArr(int *arr, size_t arr_size);
static int CompFunc(const void *new_data, 
                    const void *src_data,
					void *compare_param);

int HeapifyUpTest();
int HeapifyDownTest();

#define ARR_SIZE 15


int main ()
{	
	RUN_SEQUENCE("HeapifyUpTest", HeapifyUpTest());
	RUN_SEQUENCE("HeapifyDownTest", HeapifyDownTest());
		
	return 0;
} 

int HeapifyUpTest()
{
	size_t i = 0, j = 0;
	int arr[ARR_SIZE] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
	int res_arr[ARR_SIZE][ARR_SIZE] =
	{{15, 2, 1, 4, 5, 6, 3, 8, 9, 10, 11, 12, 13, 14, 7},
	 {14, 2, 15, 4, 5, 6, 1, 8, 9, 10, 11, 12, 13, 3, 7},
	 {13, 2, 14, 4, 5, 15, 1, 8, 9, 10, 11, 12, 6, 3, 7},
	 {12, 2, 13, 4, 5, 14, 1, 8, 9, 10, 11, 15, 6, 3, 7},
	 {11, 12, 13, 4, 2, 14, 1, 8, 9, 10, 5, 15, 6, 3, 7},
	 {10, 11, 13, 4, 12, 14, 1, 8, 9, 2, 5, 15, 6, 3, 7},
	 {9, 10, 13, 11, 12, 14, 1, 8, 4, 2, 5, 15, 6, 3, 7},
	 {8, 9, 13, 10, 12, 14, 1, 11, 4, 2, 5, 15, 6, 3, 7},
	 {1, 9, 8, 10, 12, 14, 13, 11, 4, 2, 5, 15, 6, 3, 7},
 	 {14, 9, 1, 10, 12, 8, 13, 11, 4, 2, 5, 15, 6, 3, 7},
	 {12, 14, 1, 10, 9, 8, 13, 11, 4, 2, 5, 15, 6, 3, 7},
	 {10, 12, 1, 14, 9, 8, 13, 11, 4, 2, 5, 15, 6, 3, 7},
	 {1, 12, 10, 14, 9, 8, 13, 11, 4, 2, 5, 15, 6, 3, 7},
	 {12, 1, 10, 14, 9, 8, 13, 11, 4, 2, 5, 15, 6, 3, 7}};

	for (i = ARR_SIZE - 1; 0 < i; --i, ++j)
	{
		HeapifyUp(arr, 
				    ARR_SIZE,
				    sizeof(int), 
				    i, 
				    CompFunc, 
				    NULL);

	    RUN_TEST("test", 0 == memcmp(arr, res_arr[j], sizeof(int)*ARR_SIZE));
		PrintArr(arr, ARR_SIZE);
	}

	return 0;
}


int HeapifyDownTest()
{
	size_t i = 0;
	int arr[ARR_SIZE] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
	int res_arr[ARR_SIZE][ARR_SIZE] =
	{{3, 2, 7, 4, 5, 6, 15, 8, 9, 10, 11, 12, 13, 14, 1},
	 {3, 5, 7, 4, 11, 6, 15, 8, 9, 10, 2, 12, 13, 14, 1},
	 {3, 5, 15, 4, 11, 6, 14, 8, 9, 10, 2, 12, 13, 7, 1},
	 {3, 5, 15, 9, 11, 6, 14, 8, 4, 10, 2, 12, 13, 7, 1},
	 {3, 5, 15, 9, 11, 6, 14, 8, 4, 10, 2, 12, 13, 7, 1},
	 {3, 5, 15, 9, 11, 13, 14, 8, 4, 10, 2, 12, 6, 7, 1},
	 {3, 5, 15, 9, 11, 13, 14, 8, 4, 10, 2, 12, 6, 7, 1},
	 {3, 5, 15, 9, 11, 13, 14, 8, 4, 10, 2, 12, 6, 7, 1},
	 {3, 5, 15, 9, 11, 13, 14, 8, 4, 10, 2, 12, 6, 7, 1},
	 {3, 5, 15, 9, 11, 13, 14, 8, 4, 10, 2, 12, 6, 7, 1},
	 {3, 5, 15, 9, 11, 13, 14, 8, 4, 10, 2, 12, 6, 7, 1},
	 {3, 5, 15, 9, 11, 13, 14, 8, 4, 10, 2, 12, 6, 7, 1},
	 {3, 5, 15, 9, 11, 13, 14, 8, 4, 10, 2, 12, 6, 7, 1},
	 {3, 5, 15, 9, 11, 13, 14, 8, 4, 10, 2, 12, 6, 7, 1},
	 {3, 5, 15, 9, 11, 13, 14, 8, 4, 10, 2, 12, 6, 7, 1}};

	for (i = 0; i < ARR_SIZE; ++i)
	{
		HeapifyDown(arr, 
				    ARR_SIZE,
				    sizeof(int), 
				    i, 
				    CompFunc, 
				    NULL);

	    RUN_TEST("test", 0 == memcmp(arr, res_arr[i], sizeof(int)*ARR_SIZE));
		PrintArr(arr, ARR_SIZE);
	}

	return 0;
}




static void PrintArr(int *arr, size_t arr_size)
{
    size_t i = 0;
    
    printf("\n{");
    for (i = 0; i < arr_size; ++i)
    {
        printf("%d, ", *(int *)(arr + i));
    }
    printf("END}\n");
}

static int CompFunc(const void *new_data, 
                    const void *src_data,
					void *compare_param)
{
    UNUSE(compare_param);
    
    if (*(int *)new_data > *(int *)src_data)
    {
        return (1);
    }
    else if (*(int *)src_data > *(int *)new_data)
    {
        return (-1);
    }
    else
    {
        return (0);
    }
}
