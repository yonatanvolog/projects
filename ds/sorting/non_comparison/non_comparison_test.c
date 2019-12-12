/**********************************************************************
 *	This code is representing my tests of 							  *
 *	non_comaprison sorting algorithms RC2       				      * 
 *	Lab: OL-79                                		                  *
 *	Author: Yonatan Vologdin 										  *
 *	Reviewer: Maoz Roytman	 	  								 	  *
 *	Date: 12.12.19													  *
 **********************************************************************/

#include <stdio.h>		/*printf*/
#include <stdlib.h>		/*qsort*/	
#include <string.h>		/*memcpy*/
#include <time.h>		/*time*/

#include "../sorting.h"	
#include "yoni_utils.h"	

#define ARR_SIZE 10
#define RADIX_ARR 300

int CountingSortTest();
int RadixSortTest();

static int CmpFunc(const void *a, const void *b);

int main()
{
    srand(time(NULL));

	RUN_SEQUENCE("CountingSortTest", CountingSortTest());
	RUN_SEQUENCE("RadixSortTest", RadixSortTest());

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

int RadixSortTest()
{
	int i = 0, j = 0;
	int random = 0;
	int arr1[RADIX_ARR] = {0};
	int arr2[RADIX_ARR] = {0};
	
	for (j = 0; j < 1000; ++j)
	{
		for (i = 0; i < RADIX_ARR; ++i)
		{
			random = rand() % 200;
			arr1[i] = random;
			arr2[i] = random;
		}
	
		RadixSort(arr1, RADIX_ARR);		
		qsort(arr2, RADIX_ARR, sizeof(int), CmpFunc);
		RUN_TEST("Test", 0 == memcmp(arr1, arr2, RADIX_ARR*sizeof(int)));
	}

	return 0;
}


static int CmpFunc(const void *a, const void *b)
{
   return (*((int *)a) > *((int *)b));
}
