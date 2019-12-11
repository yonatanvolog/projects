/**********************************************************************
 *	This code is representing my tests of sorting algorithms RC1      *
 *	Lab: OL-79                                		                  *
 *	Author: Yonatan Vologdin 										  *
 *	Reviewer: Maoz Roytman	 	  								 	  *
 *	Date: 10.12.19													  *
 **********************************************************************/

#include <stdio.h>		/*printf*/
#include <stdlib.h>		/*qsort*/	
#include <string.h>		/*memcpy*/
#include <time.h>		/*time*/

#include "sort_algorithms.h"
#include "yoni_utils.h"	/*utils*/	

#define ARR_SIZE 10

int BubbleSortTest();
int InsertionSortTest();
int SelectionSortTest();
int CountingSortTest();

static int CmpFunc(const void *a, const void *b);
static void InitArraysIMP(int *arr1, int *arr2, size_t size);
static void SetMinMaxValuesIMP(int *arr, int *min_val, int *max_val);


int main()
{
    srand(time(NULL));

	/*RUN_SEQUENCE("BubbleSortTest", BubbleSortTest());
	RUN_SEQUENCE("InsertionSortTest", InsertionSortTest());
	RUN_SEQUENCE("SelectionSortTest", SelectionSortTest());*/
	RUN_SEQUENCE("CountingSortTest", CountingSortTest());
	
	return 0;
}

int BubbleSortTest()
{
	int arr1[] = {1};
	int arr2[] = {1};

	int rnd_arr1[ARR_SIZE];
	int rnd_arr2[ARR_SIZE];

	size_t i = 0;

	/*Array with only 1 element*/
	BubbleSort(arr1, 1);
	qsort(arr2, 1, sizeof(int), CmpFunc);
	RUN_TEST("Test", 0 == memcmp(arr1, arr2, (1 * sizeof(int))));

	for(i = 1; i <= 1000; ++i)
	{
		InitArraysIMP(rnd_arr1, rnd_arr2, ARR_SIZE);
		memcpy(rnd_arr2, rnd_arr1, (ARR_SIZE * sizeof(int)));

		BubbleSort(rnd_arr1, ARR_SIZE);
		qsort(rnd_arr2, ARR_SIZE, sizeof(int), CmpFunc);

		RUN_TEST("Test", 0 == memcmp(rnd_arr2, rnd_arr1, (ARR_SIZE * sizeof(int))));
	}

	return 0;
}

int InsertionSortTest()
{
	int arr1[] = {1};
	int arr2[] = {1};

	int rnd_arr1[ARR_SIZE];
	int rnd_arr2[ARR_SIZE];

	size_t i = 0;

	/*Array with only 1 element*/
	BubbleSort(arr1, 1);
	qsort(arr2, 1, sizeof(int), CmpFunc);
	RUN_TEST("Test", 0 == memcmp(arr1, arr2, (1 * sizeof(int))));

	for(i = 1; i <= 1000; ++i)
	{
		InitArraysIMP(rnd_arr1, rnd_arr2, ARR_SIZE);
		memcpy(rnd_arr2, rnd_arr1, (ARR_SIZE * sizeof(int)));

		InsertionSort(rnd_arr1, ARR_SIZE);
		qsort(rnd_arr2, ARR_SIZE, sizeof(int), CmpFunc);

		RUN_TEST("Test", 0 == memcmp(rnd_arr2, rnd_arr1, (ARR_SIZE * sizeof(int))));
	}

	return 0;
}

int SelectionSortTest()
{
	int arr1[] = {1};
	int arr2[] = {1};

	int rnd_arr1[ARR_SIZE];
	int rnd_arr2[ARR_SIZE];

	size_t i = 0;

	/*Array with only 1 element*/
	BubbleSort(arr1, 1);
	qsort(arr2, 1, sizeof(int), CmpFunc);
	RUN_TEST("Test", 0 == memcmp(arr1, arr2, (1 * sizeof(int))));

	for(i = 1; i <= 1000; ++i)
	{
		InitArraysIMP(rnd_arr1, rnd_arr2, ARR_SIZE);
		memcpy(rnd_arr2, rnd_arr1, (ARR_SIZE * sizeof(int)));

		SelectionSort(rnd_arr1, ARR_SIZE);
		qsort(rnd_arr2, ARR_SIZE, sizeof(int), CmpFunc);

		RUN_TEST("Test", 0 == memcmp(rnd_arr2, rnd_arr1, (ARR_SIZE * sizeof(int))));
	}

	return 0;
}













int CountingSortTest()
{
	/*int arr1[] = {1};
	int arr2[] = {1};*/

	int rnd_arr1[ARR_SIZE];
	int rnd_arr2[ARR_SIZE];

	int min_val = 0;
	int max_val = 0;

	size_t i = 0;
	int *result = NULL;

	/*Array with only 1 element*/
/*	CountingSort(arr1, 1, 1, 1, RESULT_______________________________________);
	qsort(arr2, 1, sizeof(int), CmpFunc);
	RUN_TEST("Test", 0 == memcmp(arr1, arr2, (1 * sizeof(int))));*/


	result = (int *)malloc(ARR_SIZE * sizeof(int));
	if(NULL == result)
	{
		printf("CountingSortTest malloc failed\n");

		return 1;
	}	
	
	for(i = 1; i <= 1000; ++i)
	{
		InitArraysIMP(rnd_arr1, rnd_arr2, ARR_SIZE);
		memcpy(rnd_arr2, rnd_arr1, (ARR_SIZE * sizeof(int)));
		
		SetMinMaxValuesIMP(rnd_arr1, &min_val, &max_val);
		CountingSort(rnd_arr1, ARR_SIZE, min_val, max_val, result);
		qsort(rnd_arr2, ARR_SIZE, sizeof(int), CmpFunc);

		RUN_TEST("Test", 0 == memcmp(rnd_arr2, rnd_arr1, (ARR_SIZE * sizeof(int))));
	}

	return 0;
}

static void SetMinMaxValuesIMP(int *arr, int *min_val, int *max_val)
{
	size_t i = 0;
	
	for(i = 0; i < ARR_SIZE; ++i)
	{
		if(arr[i] > *max_val)
		{
			*max_val = arr[i]; 
		}

		if(*min_val > arr[i])
		{
			*min_val = arr[i]; 
		}
	}
}




























static int CmpFunc(const void *a, const void *b)
{
   return (*((int *)a) > *((int *)b));
}

static void InitArraysIMP(int *arr1, int *arr2, size_t size)
{
	size_t i = 0;

	for (i = 0; i < size; ++i)
	{
		arr1[i] = rand();
		arr2[i] = arr1[i];
	}
}
