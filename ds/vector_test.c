/**********************************************************************
 *	This code is representing my tests of DS_Vector RC2		          *
 *	Lab: OL-79                                		                  *
 *	Author: Yonatan Vologdin 										  *
 *	Reviewer: Erez Barr    				          				      *
 *	Date: 04.11.19													  *
 **********************************************************************/

#include <stdio.h>	/*printf, size_t*/
#include <assert.h>	/*assert*/
#include "vector.h"	/*vector structure*/

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define RUN_TEST(test_name, result) \
(printf("%s %s\n", test_name, (1 == (result)) ? KGRN"\tPassed"KNRM : KRED"\tFailed"KNRM))

int VectorSequenceTest();

int main()
{
	RUN_TEST("VectorSequenceTest", VectorSequenceTest());

	return 0;
}

int VectorSequenceTest()
{
	int n0 = 5;
	int n1 = 6;
	int n2 = 7;
	int n3 = 8;
	int n4 = 9;
	int n5 = 10;
	int n6 = 11;
	int n7 = 12;
	
	d_vector_t *v = VectorCreate(3, sizeof(int));
	
	assert(NULL != v);

	if(3 != VectorCapacity(v))
	{
		return 0;
	}
	VectorPushBack(v, &n0);
	VectorPushBack(v, &n1);
	VectorPushBack(v, &n2);
	
	if((3 != VectorCapacity(v)) && (7 != *((int *)VectorGetItemAddress(v, 2))))
	{
		return 0;
	}	
	VectorPushBack(v, &n3); /*now allocates more memmory, should be 6*/

	if((6 != VectorCapacity(v)) && (8 != *((int *)VectorGetItemAddress(v, 3))))
	{
		return 0;
	}
	VectorPushBack(v, &n4);
	VectorPushBack(v, &n5);
	
	if((3 != VectorCapacity(v)) && 
	(10 != *((int *)VectorGetItemAddress(v, 5)) &&
	(6 != VectorSize(v))))
	{
		return 0;
	}
	VectorPushBack(v, &n6); /*now allocates more memmory, should be 12*/
	
	if((12 != VectorCapacity(v)) && 
	(11 != *((int *)VectorGetItemAddress(v, 6)) &&
	(7 != VectorSize(v))))
	{
		return 0;
	}
	VectorPopBack(v);
	VectorPopBack(v);
	VectorPopBack(v);
	VectorPopBack(v);

	if((12 != VectorCapacity(v)) && 
	(8 != *((int *)VectorGetItemAddress(v, 3)) &&
	(4 != VectorSize(v))))
	{
		return 0;
	}
	VectorPopBack(v);	/*now allocates less memmory, should be 6*/

	if((6 != VectorCapacity(v)) && 
	(7 != *((int *)VectorGetItemAddress(v, 2)) &&
	(3 != VectorSize(v))))
	{
		return 0;
	}
	VectorReserve(v, 1);	/*now tries to allocate less memmory, should change to 3*/

	if(	(3 != VectorCapacity(v)	) && 
	(6 != *((int *)VectorGetItemAddress(v, 1)) &&
	(3 != VectorSize(v))))
	{
		return 0;
	}
	
	VectorReserve(v, 100);	/*now allocates more memmory, should be 100*/

	if((100 != VectorCapacity(v)) && 
	(7 != *((int *)VectorGetItemAddress(v, 2)) &&
	(3 != VectorSize(v))))
	{
		return 0;
	}
	VectorPushBack(v, &n7);	

	if((100 != VectorCapacity(v)) && 
	(12 != *((int *)VectorGetItemAddress(v, 3)) &&
	(4 != VectorSize(v))))
	{
		return 0;
	}
	VectorPopBack(v);	/*now allocates less memmory, should be 50*/

	if((50 != VectorCapacity(v)) && 
	(7 != *((int *)VectorGetItemAddress(v, 2)) &&
	(3 != VectorSize(v))))
	{
		return 0;
	}
	VectorDestroy(v);

	return 1;
}
