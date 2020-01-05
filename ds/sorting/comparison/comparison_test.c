/**********************************************************************
 *	This code is representing my tests of 							  *
 *	comaprison sorting algorithms RC2       						  * 
 *	Lab: OL-79                                		                  *
 *	Author: Yonatan Vologdin 										  *
 *	Reviewer: Maoz Roytman	 	  								 	  *
 *	Date: 11.12.19													  *
 **********************************************************************/

#include <stdio.h>		/*printf*/
#include <stdlib.h>		/*qsort*/	
#include <string.h>		/*memcpy*/
#include <time.h>		/*time*/

#include "../sorting.h"	
#include "../../../yoni_utils.h"

#define ARR_SIZE 10
#define RADIX_ARR 300

int BubbleSortTest();
int InsertionSortTest();
int SelectionSortTest();

static int CmpFunc(const void *a, const void *b);
static void InitArraysIMP(int *arr1, int *arr2, size_t size);

int main()
{
    srand(time(NULL));

	RUN_SEQUENCE("BubbleSortTest", BubbleSortTest());
	RUN_SEQUENCE("InsertionSortTest", InsertionSortTest());
	RUN_SEQUENCE("SelectionSortTest", SelectionSortTest());

	return 0;
}

int BubbleSortTest()
{
	int rnd_arr1[ARR_SIZE];
	int rnd_arr2[ARR_SIZE];
	size_t i = 0;

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
	int rnd_arr1[ARR_SIZE];
	int rnd_arr2[ARR_SIZE];
	size_t i = 0;

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
	int rnd_arr1[ARR_SIZE];
	int rnd_arr2[ARR_SIZE];
	size_t i = 0;

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
