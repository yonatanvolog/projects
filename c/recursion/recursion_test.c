/**********************************************************************
 *	This code is representing my tests of recursion RC1               *
 *	Lab: OL-79                                		                  *
 *	Author: Yonatan Vologdin 										  *
 *	Reviewer: Chen Rubin	 	  							     	  *
 *	Date: 23.12.19													  *
 **********************************************************************/

#include <stdio.h>	/*printf*/
#include <string.h>

#include "recursion.h"
#include "../../yoni_utils.h"

int IterativeFibonacciTest();
int RecursiveFibonacciTest();
int RecStrlenTest();
int RecStrcmpTest();
int RecStrcpyTest();
int RecStrcatTest();
int RecStrstrTest();
int RecFlipTest();
int RecSortStackTest();

int main()
{
	RUN_SEQUENCE("IterativeFibonacciTest", IterativeFibonacciTest());
	RUN_SEQUENCE("RecursiveFibonacciTest", RecursiveFibonacciTest());
	RUN_SEQUENCE("RecStrlenTest", RecStrlenTest());
	RUN_SEQUENCE("RecStrcmpTest", RecStrcmpTest());
	RUN_SEQUENCE("RecStrcpyTest", RecStrcpyTest());
	RUN_SEQUENCE("RecStrcatTest", RecStrcatTest());
	RUN_SEQUENCE("RecStrstrTest", RecStrstrTest());
	RUN_SEQUENCE("RecFlipTest", RecFlipTest());
	RUN_SEQUENCE("RecSortStackTest", RecSortStackTest());

	return 0;
}

int IterativeFibonacciTest()
{
	RUN_TEST("0th", 0 == IterativeFibonacci(0));
	RUN_TEST("1st", 1 == IterativeFibonacci(1));
	RUN_TEST("2st", 1 == IterativeFibonacci(2));
	RUN_TEST("3rd", 2 == IterativeFibonacci(3));
	RUN_TEST("10nth", 55 == IterativeFibonacci(10));
	RUN_TEST("50th", 2178309 == IterativeFibonacci(32));

	return 0;
}

int RecursiveFibonacciTest()
{
	RUN_TEST("0th", 0 == RecursiveFibonacci(0));
	RUN_TEST("1st", 1 == RecursiveFibonacci(1));
	RUN_TEST("2st", 1 == RecursiveFibonacci(2));
	RUN_TEST("3rd", 2 == RecursiveFibonacci(3));
	RUN_TEST("10nth", 55 == RecursiveFibonacci(10));
	RUN_TEST("50th", 2178309 == RecursiveFibonacci(32));

	return 0;
}

int RecStrlenTest()
{
	char str1[] = "";
	char str2[] = "a";
	char str3[] = "abc";

	RUN_TEST("empty", strlen(str1) == RecStrlen(str1));
	RUN_TEST("a", strlen(str2) == RecStrlen(str2));
	RUN_TEST("abc", strlen(str3) == RecStrlen(str3));

	return 0;
}

int RecStrcmpTest()
{
	char str11[] = "eyal";
	char str12[] = "elya";
	char str21[] = "a";
	char str22[] = "a";
	char str31[] = "abc";
	char str32[] = "abc";
	char str41[] = "";
	char str42[] = "a";
	char str51[] = "ab";
	char str52[] = "abc";

	RUN_TEST("empty empty", strcmp(str11, str12) == RecStrcmp(str11, str12));
	RUN_TEST("a a\t", strcmp(str21, str22) == RecStrcmp(str21, str22));
	RUN_TEST("abc abc", strcmp(str31, str32) == RecStrcmp(str31, str32));
	RUN_TEST("empty a", strcmp(str41, str42) == RecStrcmp(str41, str42));
	RUN_TEST("ab abc\t", strcmp(str51, str52) == RecStrcmp(str51, str52));

	return 0;
}

int RecStrcpyTest()
{
	char strsrc11[] = "";
	char strdest11[] = "";
	char strsrc12[] = "";
	char strdest12[] = "";

	char strsrc21[] = "a";
	char strdest21[] = "a";
	char strsrc22[] = "a";
	char strdest22[] = "a";

	char strsrc31[] = "123";
	char strdest31[] = "abcfd";
	char strsrc32[] = "123";
	char strdest32[] = "abcfd";

	char strsrc41[] = "adsad";
	char strdest41[] = "cdcaaaaaaa";
	char strsrc42[] = "adsad";
	char strdest42[] = "cdcaaaaaaa";

	char strsrc51[] = "";
	char strdest51[] = "wer";
	char strsrc52[] = "";
	char strdest52[] = "wer";

	char strsrc61[] = "hello";
	char strdest61[] = "alo1234";
	char strsrc62[] = "hello";
	char strdest62[] = "alo1234";

	char strsrc71[] = "alo";
	char strdest71[] = "hello";
	char strsrc72[] = "alo";
	char strdest72[] = "hello";

	RUN_TEST("empty to empty", 0 == strcmp(strcpy(strdest12, strsrc12),
									RecStrcpy(strdest11, strsrc11)));
	RUN_TEST("char to char", 0 == strcmp(strcpy(strdest22, strsrc22),
									RecStrcpy(strdest21, strsrc21)));
	RUN_TEST("3 to 5\t", 0 == strcmp(strcpy(strdest32, strsrc32),
									RecStrcpy(strdest31, strsrc31)));
	RUN_TEST("5 to 10\t", 0 == strcmp(strcpy(strdest42, strsrc42),
									RecStrcpy(strdest41, strsrc41)));
	RUN_TEST("empty to 3", 0 == strcmp(strcpy(strdest52, strsrc52),
									RecStrcpy(strdest51, strsrc51)));
	RUN_TEST("5 ot 7\t", 0 == strcmp(strcpy(strdest62, strsrc62),
									RecStrcpy(strdest61, strsrc61)));
	RUN_TEST("3 to 5\t", 0 == strcmp(strcpy(strdest72, strsrc72),
									RecStrcpy(strdest71, strsrc71)));

	return 0;
}

int RecStrcatTest()
{
	char strsrc11[1] = "";
	char strdest11[30] = "";
	char strsrc12[1] = "";
	char strdest12[30] = "";

	char strsrc21[2] = "a";
	char strdest21[30] = "a";
	char strsrc22[2] = "a";
	char strdest22[30] = "a";

	char strsrc31[4] = "123";
	char strdest31[30] = "abcfd";
	char strsrc32[4] = "123";
	char strdest32[30] = "abcfd";


	RUN_TEST("empty to empty", 0 == strcmp(strcat(strdest12, strsrc12),
									RecStrcat(strdest11, strsrc11)));
	RUN_TEST("aa\t", 0 == strcmp(strcat(strdest22, strsrc22),
									RecStrcat(strdest21, strsrc21)));
	RUN_TEST("abcfd123", 0 == strcmp(strcat(strdest32, strsrc32),
									RecStrcat(strdest31, strsrc31)));


	return 0;
}

int RecStrstrTest()
{
	char needle1[] = "";
	char haystack1[] = "";

	char needle2[] = "a";
	char haystack2[] = "a";

	char needle3[] = "123";
	char haystack3[] = "abcfd";

	char needle4[] = "abc";
	char haystack4[] = "abcfd";

	char needle5[] = "fda";
	char haystack5[] = "abcfd";

	RUN_TEST("empty to empty", strstr(haystack1, needle1) ==
									RecStrstr(haystack1, needle1));

	RUN_TEST("a in a\t", 0 == strcmp(strstr(haystack2, needle2),
									RecStrstr(haystack2, needle2)));

	RUN_TEST("123 in abcfd", strstr(haystack3, needle3) ==
									RecStrstr(haystack3, needle3));

	RUN_TEST("abc in abcfd", 0 == strcmp(strstr(haystack4, needle4),
									RecStrstr(haystack4, needle4)));

	RUN_TEST("fda in abcfd", strstr(haystack5, needle5) == 
									RecStrstr(haystack5, needle5));

	return 0;	
}


int RecFlipTest()
{
	int num1 = 1;
	int num2 = 2;
	int num3 = 3;
	int num4 = 4;	
	int num5 = 5;

	/*node1->node2->node3->node4->node5->NULL*/
	sl_node_t *node5 = SListCreateNode(&num5, NULL);
	sl_node_t *node4 = SListCreateNode(&num4, node5);
	sl_node_t *node3 = SListCreateNode(&num3, node4);
	sl_node_t *node2 = SListCreateNode(&num2, node3);
	sl_node_t *node1 = SListCreateNode(&num1, node2);

	sl_node_t *new_head = RecFlip(node1);
	
	/*NULL<-node1<-node2<-node3<-node4<-node5(head)*/
	RUN_TEST("node5 is head", new_head == node5);
	RUN_TEST("node5 next", node4 == node5->next);
	RUN_TEST("node4 next", node3 == node4->next);
	RUN_TEST("node3 next", node2 == node3->next);
	RUN_TEST("node2 next", node1 == node2->next);
	RUN_TEST("node1 next", NULL == node1->next);

	SListFreeAll(new_head);

	return 0;
}

static int CmpFunc(void *data1, void *data2)
{
	if (*(int *)data1 > *(int *)data2)
	{
		return 1;
	}

	else if (*(int *)data1 < *(int *)data2)
	{
		return -1;
	}
		
	return 0;
}

int RecSortStackTest()
{
	int num1 = 1;
	int num2 = 2;
	int num3 = 3;
	int num4 = 4;
	int num5 = 5;

	int test = 0;

	stack_t *stack1 = StackCreate(5, sizeof(int));
	if(NULL == stack1)
	{
		return 1;
	}
															
	test += StackPush(stack1, &num1);       /* stack representation	*/
	test += StackPush(stack1, &num3);       /*----------------------*/
	test += StackPush(stack1, &num5);       /*top| 2,4,5,3,1 |bottom*/
	test += StackPush(stack1, &num4);
	test += StackPush(stack1, &num2);
	if(0 != test)
	{
		return 1;
	}

	RecSortStack(stack1, sizeof(int *), CmpFunc);
	
	RUN_TEST("peek1", num1 == *(int *)StackPeek(stack1));
	StackPop(stack1);
	RUN_TEST("peek2", num2 == *(int *)StackPeek(stack1));
	StackPop(stack1);
	RUN_TEST("peek3", num3 == *(int *)StackPeek(stack1));
	StackPop(stack1);
	RUN_TEST("peek4", num4 == *(int *)StackPeek(stack1));
	StackPop(stack1);
	RUN_TEST("peek5", num5 == *(int *)StackPeek(stack1));
	StackPop(stack1);
	RUN_TEST("is empty?", 1 == StackIsEmpty(stack1));
	
	StackDestroy(stack1);

	return 0;
}
