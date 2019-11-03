/**********************************************************************
 *	This code is representing my test of DS_Stack             	      *
 *	Lab: OL-79                                		                  *
 *	Author: Yonatan Vologdin 										  *
 *	Reviewer: Sharon Rottner    								      *
 *	Date: 02.11.19													  *
 **********************************************************************/

#include <stdio.h>	/*printf, size_t*/
#include <assert.h>	/*assert*/
#include "stack.h"	/*stack structure*/

#define RESULT(condition) (condition) ? 1 : 0
#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define RUN_TEST(test_name, result) \
(printf("%s %s\n", test_name, (1 == (result)) ? KGRN"\tPassed"KNRM : KRED"\tFailed"KNRM))

int StackSequenceTest();

int main()
{
	RUN_TEST("StackSequenceTest", StackSequenceTest());
	
	return 0;
}

int StackSequenceTest()
{
	int n1 = 5;
	int n2 = 6;
	int n3 = 7;
	int n4 = 8;
	int res = 0;
	stack_t *s = StackCreate(3, sizeof(int));

	assert(NULL != s);
	
	if(1 != StackIsEmpty(s) && 0 == StackSize(s)) /*just created, should be empty*/
	{
		return 0;
	}
	res = StackPush(s, &n1);
	if	(1 == res && 1 == StackSize(s)) /*failed to push, should succeed*/
	{
		return 0;
	}
	res = StackPush(s, &n2);	
	if	(1 == res && 2 == StackSize(s)) /*failed to push, should succeed*/
	{
		return 0;
	}	 
	res = StackPush(s, &n3);
	if	(1 == res && 3 == StackSize(s)) /*failed to push, should succeed*/
	{
		return 0;
	}	
	res = StackPush(s, &n4);
	if	(0 == res && 3 == StackSize(s)) /*failed to push, should fail*/
	{
		return 0;
	}	  
	
	if(n3 != *(int *)StackPeek(s) && 3 == StackSize(s))
	{
		return 0;
	}
	StackPop(s);
	if(n2 != *(int *)StackPeek(s) && 2 == StackSize(s))
	{
		return 0;
	}
	StackPop(s);
	if(n1 != *(int *)StackPeek(s) && 1 == StackSize(s))
	{
		return 0;
	}
	StackPop(s);
	if(1 != StackIsEmpty(s) && 0 == StackSize(s)) /*all elements poped out, should be empty*/
	{
		return 0;
	}
	StackDestroy(s);

	return 1;
}
