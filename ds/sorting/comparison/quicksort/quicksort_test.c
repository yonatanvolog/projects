/**********************************************************************
 *	This code is representing my tests of Quick Sort 	              *
 *	Lab: OL-79                                		                  *
 *	Author: Yonatan Vologdin 										  *
 *	Reviewer: Dvir Breitbart 	 	  						     	  *
 *	Date: 28.12.19													  *
 **********************************************************************/

#include <stdio.h>	/*printf*/
#include <string.h>	/*memcmp*/
#include <stdlib.h>	/*qsort*/

#include "../../sorting.h"
#include "../../../../yoni_utils.h"

int QuickSortTest();

int CmpFunc(const void *a, const void *b)
{
	return ((*(int *)a - *(int *)b));
}

int main()
{
	RUN_SEQUENCE("QuickSortTest", QuickSortTest());

	return 0;
}

int QuickSortTest()
{
	int arr1m[1] = {1};
	int arr1q[1] = {1};

	int arr2m[2] = {2, 1};
	int arr2q[2] = {2, 1};

	int arr3m[8] = {2, 8, 5, 3, 9, 4, 1, 7};
	int arr3q[8] = {2, 8, 5, 3, 9, 4, 1, 7};

	int arr4m[8] = {7, 2, 6, 9, 3, 5, 1, 8};
	int arr4q[8] = {7, 2, 6, 9, 3, 5, 1, 8};

	int arr5m[7] = {38, 27, 43, 3, 9, 82, 10};
	int arr5q[7] = {38, 27, 43, 3, 9, 82, 10};

	QuickSort(arr1m, 1, sizeof(int), CmpFunc);
	QuickSort(arr2m, 2, sizeof(int), CmpFunc);
	QuickSort(arr3m, 8, sizeof(int), CmpFunc);
	QuickSort(arr4m, 8, sizeof(int), CmpFunc);
	QuickSort(arr5m, 7, sizeof(int), CmpFunc);

	qsort(arr1q, 1, sizeof(int), CmpFunc);
	qsort(arr2q, 2, sizeof(int), CmpFunc);
	qsort(arr3q, 8, sizeof(int), CmpFunc);
	qsort(arr4q, 8, sizeof(int), CmpFunc);
	qsort(arr5q, 7, sizeof(int), CmpFunc);

	RUN_TEST("arr1", 0 == memcmp(arr1m, arr1q, sizeof(int) * 1));
	RUN_TEST("arr2", 0 == memcmp(arr2m, arr2q, sizeof(int) * 2));
	RUN_TEST("arr3", 0 == memcmp(arr3m, arr3q, sizeof(int) * 8));
	RUN_TEST("arr4", 0 == memcmp(arr4m, arr4q, sizeof(int) * 8));
	RUN_TEST("arr5", 0 == memcmp(arr5m, arr5q, sizeof(int) * 7));

	return 0;
}

