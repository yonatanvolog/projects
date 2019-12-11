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


int main()
{
    srand(time(NULL));

	RUN_SEQUENCE("BubbleSortTest", BubbleSortTest());
	RUN_SEQUENCE("InsertionSortTest", InsertionSortTest());
	RUN_SEQUENCE("SelectionSortTest", SelectionSortTest());
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
	int i = 0, j = 0;
	int random = 0;
	int arr1[300] = {0};
	int arr2[300] = {0};
	int result[300] = {0};
	int min = 0;
	int max = 0;
	
	for (j = 0; j < 1000; ++j)
	{
		for (i = 0; i < 300; ++i)
		{
			random = rand() % 200;
			arr1[i] = random;
			arr2[i] = random;
		}
		
		min = arr1[0];
		max = arr1[0];

		for (i = 0; i < 300; ++i)
		{
			min = MIN(min, arr1[i]);
			max = MAX(max, arr1[i]);
		}
		
		CountingSort(arr1, 300, min, max, result);
		qsort(arr2, 300, sizeof(int), CmpFunc);
		RUN_TEST("Test", 0 == memcmp(result, arr2, 300*sizeof(int)));
	}

	return 0;
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
