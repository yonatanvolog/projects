/**********************************************************************
 *	This code is representing my tests of searching     	          *
 *	Lab: OL-79                                		                  *
 *	Author: Yonatan Vologdin 										  *
 *	Date: 07.01.20 													  *
 **********************************************************************/
#include <stdio.h>	    /*printf*/

#include "searching.h"
#include "../../../yoni_utils.h" /*RUN_SEQUENCE, RUN_TEST*/

int BSearchTest();
int JSearchTest();

int main()
{
	RUN_SEQUENCE("BSearchTest", BSearchTest());
	RUN_SEQUENCE("JSearchTest", JSearchTest());

	return 0;
}

int BSearchTest()
{
    int arr1[] = {1};
    int arr2[] = {1, 2};
    int arr3[] = {1, 2, 3};
    int arr4[] = {1, 2, 3, 4};
    int arr5[] = {1, 2, 3, 4, 5};
	
	RUN_TEST("Test1.1", arr1 == BSearch(arr1, 1, 1));
	RUN_TEST("Test1.2", NULL == BSearch(arr1, 1, 2));
	
	RUN_TEST("Test2.1", &arr2[0] == BSearch(arr2, 2, 1));
	RUN_TEST("Test2.2", &arr2[1] == BSearch(arr2, 2, 2));
	RUN_TEST("Test2.3",     NULL == BSearch(arr2, 2, 3));
	
	RUN_TEST("Test3.1", &arr3[0] == BSearch(arr3, 3, 1));
	RUN_TEST("Test3.2", &arr3[1] == BSearch(arr3, 3, 2));
	RUN_TEST("Test3.3", &arr3[2] == BSearch(arr3, 3, 3));
	RUN_TEST("Test3.4",     NULL == BSearch(arr3, 3, 4));
	
	RUN_TEST("Test4.1", &arr4[0] == BSearch(arr4, 4, 1));
	RUN_TEST("Test4.2", &arr4[1] == BSearch(arr4, 4, 2));
	RUN_TEST("Test4.3", &arr4[2] == BSearch(arr4, 4, 3));
	RUN_TEST("Test4.4", &arr4[3] == BSearch(arr4, 4, 4));
	RUN_TEST("Test4.5",     NULL == BSearch(arr4, 4, 5));
	
	RUN_TEST("Test5.1", &arr5[0] == BSearch(arr5, 5, 1));
	RUN_TEST("Test5.2", &arr5[1] == BSearch(arr5, 5, 2));
	RUN_TEST("Test5.3", &arr5[2] == BSearch(arr5, 5, 3));
	RUN_TEST("Test5.4", &arr5[3] == BSearch(arr5, 5, 4));
	RUN_TEST("Test5.5", &arr5[4] == BSearch(arr5, 5, 5));
	RUN_TEST("Test5.6",     NULL == BSearch(arr5, 5, 6));
	
	return 0;
}

int JSearchTest()
{
    int arr1[] = {1};
    int arr2[] = {1, 2};
    int arr3[] = {1, 2, 3};
    int arr4[] = {1, 2, 3, 4};
    int arr5[] = {1, 2, 3, 4, 5};
	
	RUN_TEST("Test1.1", arr1 == JSearch(arr1, 1, 1));
	RUN_TEST("Test1.2", NULL == JSearch(arr1, 1, 2));
	
	RUN_TEST("Test2.1", &arr2[0] == JSearch(arr2, 2, 1));
	RUN_TEST("Test2.2", &arr2[1] == JSearch(arr2, 2, 2));
	RUN_TEST("Test2.3",     NULL == JSearch(arr2, 2, 3));
	
	RUN_TEST("Test3.1", &arr3[0] == JSearch(arr3, 3, 1));
	RUN_TEST("Test3.2", &arr3[1] == JSearch(arr3, 3, 2));
	RUN_TEST("Test3.3", &arr3[2] == JSearch(arr3, 3, 3));
	RUN_TEST("Test3.4",     NULL == JSearch(arr3, 3, 4));
	
	RUN_TEST("Test4.1", &arr4[0] == JSearch(arr4, 4, 1));
	RUN_TEST("Test4.2", &arr4[1] == JSearch(arr4, 4, 2));
	RUN_TEST("Test4.3", &arr4[2] == JSearch(arr4, 4, 3));
	RUN_TEST("Test4.4", &arr4[3] == JSearch(arr4, 4, 4));
	RUN_TEST("Test4.5",     NULL == JSearch(arr4, 4, 5));
	
	RUN_TEST("Test5.1", &arr5[0] == JSearch(arr5, 5, 1));
	RUN_TEST("Test5.2", &arr5[1] == JSearch(arr5, 5, 2));
	RUN_TEST("Test5.3", &arr5[2] == JSearch(arr5, 5, 3));
	RUN_TEST("Test5.4", &arr5[3] == JSearch(arr5, 5, 4));
	RUN_TEST("Test5.5", &arr5[4] == JSearch(arr5, 5, 5));
	RUN_TEST("Test5.6",     NULL == JSearch(arr5, 5, 6));
	
	return 0;
}
















