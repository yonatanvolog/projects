/**********************************************************************
 *	This code is representing my tests BST RC1    				      * 
 *	Lab: OL-79                                		                  *
 *	Author: Yonatan Vologdin 										  *
 *	Reviewer: Sharon Rottner	 	  								  *
 *	Date: 15.12.19													  *
 **********************************************************************/

#include <stdio.h>	/*printf*/
#include <stdlib.h>

#include "bst.h"	
#include "../../yoni_utils.h"


#define WHITE_BOX_TEST

int BSTCreateTest();
int BSTDestroyTest();
int BSTInsertTest();
int BSTRemoveTest();
int BSTFindTest();
int BSTIsEmptyTest();
int BSTSizeTest();
int BSTForEachTest();
int BSTNextTest();
int BSTPrevTest();
int BSTBeginTest();
int BSTEndTest();
int BSTGetDataTest();
int BSTIsSameIteratorTest();

#ifdef WHITE_BOX_TEST
struct bst_node
{
	struct bst_node *parent;
	struct bst_node *left;
	struct bst_node *right;
	void *data;
};

struct bst
{
	comparison_func comparison_func_ptr;
	void *comparison_param;
	struct bst_node stub;
};
#endif

static int CounterFuncIMP(void *data ,void *counter);
static int ComparisonFunc(const void *new_data, 
					 	  const void *src_data, 
					  	  const void *comp_param);

int main()
{
	RUN_SEQUENCE("BSTCreateTest", BSTCreateTest());
	RUN_SEQUENCE("BSTDestroyTest", BSTDestroyTest());
	RUN_SEQUENCE("BSTInsertTest", BSTInsertTest());
	RUN_SEQUENCE("BSTRemoveTest", BSTRemoveTest());
	RUN_SEQUENCE("BSTFindTest", BSTFindTest());
	RUN_SEQUENCE("BSTIsEmptyTest", BSTIsEmptyTest());
	RUN_SEQUENCE("BSTSizeTest", BSTSizeTest());
	RUN_SEQUENCE("BSTForEachTest", BSTForEachTest());
	RUN_SEQUENCE("BSTNextTest", BSTNextTest());
	RUN_SEQUENCE("BSTPrevTest", BSTPrevTest());
	RUN_SEQUENCE("BSTBeginTest", BSTBeginTest());
#ifdef WHITE_BOX_TEST
	RUN_SEQUENCE("BSTEndTest", BSTEndTest());
#endif
	RUN_SEQUENCE("BSTGetDataTest", BSTGetDataTest());
	RUN_SEQUENCE("BSTIsSameIteratorTest", BSTIsSameIteratorTest());

	return 0;
}

int BSTCreateTest()
{
	bst_t *bst = BSTCreate(ComparisonFunc, NULL);

	printf("----Empty Test----\n");

	free(bst);
	
	return 0;
}

int BSTDestroyTest()
{
	int num1 = 8;

	int num2 = 3;
	int num3 = 10;

	int num4 = 1;
	int num5 = 6;
	int num6 = 14;
	
	int num7 = 4;
	int num8 = 7;
	int num9 = 13;

	bst_t *bst = BSTCreate(ComparisonFunc, NULL);
	
	BSTInsert(bst, &num1);
	BSTInsert(bst, &num2);
	BSTInsert(bst, &num3);
	BSTInsert(bst, &num4);
	BSTInsert(bst, &num5);
	BSTInsert(bst, &num6);
	BSTInsert(bst, &num7);
	BSTInsert(bst, &num8);
	BSTInsert(bst, &num9);

	BSTDestroy(bst);

	printf("----Check Valgrind----\n");
	
	return 0;
}

int BSTInsertTest()
{
	int num1 = 8;

	int num2 = 3;
	int num3 = 10;

	int num4 = 1;
	int num5 = 6;
	int num6 = 14;
	
	int num7 = 4;
	int num8 = 7;
	int num9 = 13;

	bst_iter_t insert1 = NULL;
	bst_iter_t insert2 = NULL;
	bst_iter_t insert3 = NULL;
	bst_iter_t insert4 = NULL;
	bst_iter_t insert5 = NULL;
	bst_iter_t insert6 = NULL;
	bst_iter_t insert7 = NULL;
	bst_iter_t insert8 = NULL;
	bst_iter_t insert9 = NULL;

	bst_t *bst = BSTCreate(ComparisonFunc, NULL);
	
	insert1 = BSTInsert(bst, &num1);
	insert2 = BSTInsert(bst, &num2);
	insert3 = BSTInsert(bst, &num3);
	insert4 = BSTInsert(bst, &num4);
	insert5 = BSTInsert(bst, &num5);
	insert6 = BSTInsert(bst, &num6);
	insert7 = BSTInsert(bst, &num7);
	insert8 = BSTInsert(bst, &num8);
	insert9 = BSTInsert(bst, &num9);

	RUN_TEST("Insert1", num1 == *((int *)BSTGetData(insert1)));
	RUN_TEST("Insert2", num2 == *((int *)BSTGetData(insert2)));
	RUN_TEST("Insert3", num3 == *((int *)BSTGetData(insert3)));
	RUN_TEST("Insert4", num4 == *((int *)BSTGetData(insert4)));
	RUN_TEST("Insert5", num5 == *((int *)BSTGetData(insert5)));
	RUN_TEST("Insert6", num6 == *((int *)BSTGetData(insert6)));
	RUN_TEST("Insert7", num7 == *((int *)BSTGetData(insert7)));
	RUN_TEST("Insert8", num8 == *((int *)BSTGetData(insert8)));
	RUN_TEST("Insert9", num9 == *((int *)BSTGetData(insert9)));

#ifdef WHITE_BOX_TEST
	printf("\n");
	RUN_TEST("stub left", (bst->stub).left == insert1);

	RUN_TEST("insert1 parent", &(bst->stub) == insert1->parent);
	RUN_TEST("insert1 left", insert2 == insert1->left);
	RUN_TEST("insert1 right", insert3 == insert1->right);

	RUN_TEST("insert2 parent", insert1 == insert2->parent);
	RUN_TEST("insert2 left", insert4 == insert2->left);
	RUN_TEST("insert2 right", insert5 == insert2->right);

	RUN_TEST("insert3 parent", insert1 == insert3->parent);
	RUN_TEST("insert3 left", NULL == insert3->left);
	RUN_TEST("insert3 right", insert6 == insert3->right);

	RUN_TEST("insert4 parent", insert2 == insert4->parent);
	RUN_TEST("insert4 left", NULL == insert4->left);
	RUN_TEST("insert4 right", NULL == insert4->right);

	RUN_TEST("insert5 parent", insert2 == insert5->parent);
	RUN_TEST("insert5 left", insert7 == insert5->left);
	RUN_TEST("insert5 right", insert8 == insert5->right);
	
	RUN_TEST("insert6 parent", insert3 == insert6->parent);
	RUN_TEST("insert6 left", insert9 == insert6->left);
	RUN_TEST("insert6 right (end)", NULL == insert6->right);

	RUN_TEST("insert7 parent", insert5 == insert7->parent);
	RUN_TEST("insert7 left", NULL == insert7->left);
	RUN_TEST("insert7 right", NULL == insert7->right);

	RUN_TEST("insert8 parent", insert5 == insert8->parent);
	RUN_TEST("insert8 left", NULL == insert8->left);
	RUN_TEST("insert8 right", NULL == insert8->right);

	RUN_TEST("insert9 parent", insert6 == insert9->parent);
	RUN_TEST("insert9 left", NULL == insert9->left);
	RUN_TEST("insert9 right", NULL == insert9->right);
#endif

	free(insert1);
	free(insert2);
	free(insert3);
	free(insert4);
	free(insert5);
	free(insert6);
	free(insert7);
	free(insert8);
	free(insert9);
	free(bst);

	return 0; 
}

int BSTRemoveTest()
{
	int num1 = 8;

	int num2 = 3;
	int num3 = 10;

	int num4 = 1;
	int num5 = 6;
	int num6 = 14;
	
	int num7 = 4;
	int num8 = 7;
	int num9 = 13;

	bst_iter_t insert1 = NULL;
	bst_iter_t insert2 = NULL;
	bst_iter_t insert3 = NULL;
	bst_iter_t insert4 = NULL;
	bst_iter_t insert5 = NULL;
	bst_iter_t insert6 = NULL;
	bst_iter_t insert7 = NULL;
	bst_iter_t insert8 = NULL;
	bst_iter_t insert9 = NULL;

	bst_t *bst = BSTCreate(ComparisonFunc, NULL);
	
	insert1 = BSTInsert(bst, &num1);
	insert2 = BSTInsert(bst, &num2);
	insert3 = BSTInsert(bst, &num3);
	insert4 = BSTInsert(bst, &num4);
	insert5 = BSTInsert(bst, &num5);
	insert6 = BSTInsert(bst, &num6);
	insert7 = BSTInsert(bst, &num7);
	insert8 = BSTInsert(bst, &num8);
	insert9 = BSTInsert(bst, &num9);

	RUN_TEST("size_check_init", 9 == BSTSize(bst));
	BSTRemove(insert7);
	RUN_TEST("size_check_rm_leaf7", 8 == BSTSize(bst));
	BSTRemove(insert5);
	RUN_TEST("size_check_rm_one_child5", 7 == BSTSize(bst));
	BSTRemove(insert3);
	RUN_TEST("size_check_rm_one_child3", 6 == BSTSize(bst));
	BSTRemove(insert2);
	RUN_TEST("size_check_rm_two_child2", 5 == BSTSize(bst));
	BSTRemove(insert1);
	RUN_TEST("size_check_rm_two_child1", 4 == BSTSize(bst));

	free(insert4);
	free(insert6);
	free(insert8);
	free(insert9);
	free(bst);

	return 0;
}

int BSTFindTest()
{
	int num1 = 8;

	int num2 = 3;
	int num3 = 10;

	int num4 = 1;
	int num5 = 6;
	int num6 = 14;
	
	int num7 = 4;
	int num8 = 7;
	int num9 = 13;
	
	int no_num = 99;

	bst_iter_t insert1 = NULL;
	bst_iter_t insert2 = NULL;
	bst_iter_t insert3 = NULL;
	bst_iter_t insert4 = NULL;
	bst_iter_t insert5 = NULL;
	bst_iter_t insert6 = NULL;
	bst_iter_t insert7 = NULL;
	bst_iter_t insert8 = NULL;
	bst_iter_t insert9 = NULL;

	bst_t *bst = BSTCreate(ComparisonFunc, NULL);
	
	insert1 = BSTInsert(bst, &num1);
	insert2 = BSTInsert(bst, &num2);
	insert3 = BSTInsert(bst, &num3);
	insert4 = BSTInsert(bst, &num4);
	insert5 = BSTInsert(bst, &num5);
	insert6 = BSTInsert(bst, &num6);
	insert7 = BSTInsert(bst, &num7);
	insert8 = BSTInsert(bst, &num8);
	insert9 = BSTInsert(bst, &num9);

	RUN_TEST("find_insert1\t", insert1 == BSTFind(bst, &num1));
	RUN_TEST("find_insert2\t", insert2 == BSTFind(bst, &num2));	
	RUN_TEST("find_insert3\t", insert3 == BSTFind(bst, &num3));
	RUN_TEST("find_insert4\t", insert4 == BSTFind(bst, &num4));
	RUN_TEST("find_insert5\t", insert5 == BSTFind(bst, &num5));
	RUN_TEST("find_insert6\t", insert6 == BSTFind(bst, &num6));
	RUN_TEST("find_insert7\t", insert7 == BSTFind(bst, &num7));
	RUN_TEST("find_insert8\t", insert8 == BSTFind(bst, &num8));
	RUN_TEST("find_insert9\t", insert9 == BSTFind(bst, &num9));
	RUN_TEST("find_non_existent", BSTEnd(bst) == BSTFind(bst, &no_num));

	free(insert1);
	free(insert2);
	free(insert3);
	free(insert4);
	free(insert5);
	free(insert6);
	free(insert7);
	free(insert8);
	free(insert9);
	free(bst);
	
	return 0;	
}

int BSTIsEmptyTest()
{	
	int num1 = 8;
	bst_iter_t to_free = NULL;
		
	bst_t *bst = BSTCreate(ComparisonFunc, NULL);
	
	RUN_TEST("empty_tree_test", 1 == BSTIsEmpty(bst));
	to_free = BSTInsert(bst, &num1);
	RUN_TEST("non_empty_tree_test", 0 == BSTIsEmpty(bst));

	free(to_free);
	free(bst);
	
	return 0;
}

int BSTSizeTest()
{
	int num1 = 8;

	int num2 = 3;
	int num3 = 10;

	int num4 = 1;
	int num5 = 6;
	int num6 = 14;
	
	int num7 = 4;
	int num8 = 7;
	int num9 = 13;

	bst_iter_t insert1 = NULL;
	bst_iter_t insert2 = NULL;
	bst_iter_t insert3 = NULL;
	bst_iter_t insert4 = NULL;
	bst_iter_t insert5 = NULL;
	bst_iter_t insert6 = NULL;
	bst_iter_t insert7 = NULL;
	bst_iter_t insert8 = NULL;
	bst_iter_t insert9 = NULL;

	bst_t *bst = BSTCreate(ComparisonFunc, NULL);
	
	RUN_TEST("size_when_empty", 0 == BSTSize(bst));
	insert1 = BSTInsert(bst, &num1);
	RUN_TEST("size_when_1\t", 1 == BSTSize(bst));
	insert2 = BSTInsert(bst, &num2);
	RUN_TEST("size_when_2\t", 2 == BSTSize(bst));
	insert3 = BSTInsert(bst, &num3);
	RUN_TEST("size_when_3\t", 3 == BSTSize(bst));
	insert4 = BSTInsert(bst, &num4);
	RUN_TEST("size_when_4\t", 4 == BSTSize(bst));
	insert5 = BSTInsert(bst, &num5);
	RUN_TEST("size_when_5\t", 5 == BSTSize(bst));
	insert6 = BSTInsert(bst, &num6);
	RUN_TEST("size_when_6\t", 6 == BSTSize(bst));
	insert7 = BSTInsert(bst, &num7);
	RUN_TEST("size_when_7\t", 7 == BSTSize(bst));
	insert8 = BSTInsert(bst, &num8);
	RUN_TEST("size_when_8\t", 8 == BSTSize(bst));
	insert9 = BSTInsert(bst, &num9);
	RUN_TEST("size_when_9(full)", 9 == BSTSize(bst));
	
	free(insert1);
	free(insert2);
	free(insert3);
	free(insert4);
	free(insert5);
	free(insert6);
	free(insert7);
	free(insert8);
	free(insert9);
	free(bst);

	return 0;
}

int BSTForEachTest()
{
	int num1 = 8;

	int num2 = 3;
	int num3 = 10;

	int num4 = 1;
	int num5 = 6;
	int num6 = 14;
	
	int num7 = 4;
	int num8 = 7;
	int num9 = 13;

	bst_iter_t insert1 = NULL;
	bst_iter_t insert2 = NULL;
	bst_iter_t insert3 = NULL;
	bst_iter_t begin = NULL;
	bst_iter_t insert4 = NULL;
	bst_iter_t insert5 = NULL;
	bst_iter_t insert6 = NULL;
	bst_iter_t insert7 = NULL;
	bst_iter_t insert8 = NULL;

	size_t counter = 0;

	bst_t *bst = BSTCreate(ComparisonFunc, NULL);
	
	insert1 = BSTInsert(bst, &num1);
	insert2 = BSTInsert(bst, &num2);
	insert3 = BSTInsert(bst, &num3);
	begin = BSTInsert(bst, &num4);
	insert4 = BSTInsert(bst, &num5);
	insert5 = BSTInsert(bst, &num6);
	insert6 = BSTInsert(bst, &num7);
	insert7 = BSTInsert(bst, &num8);
	insert8 = BSTInsert(bst, &num9);

	BSTForEach(begin, BSTEnd(bst), CounterFuncIMP, &counter);

	RUN_TEST("counter_with_foreach", 9 == counter);

	free(insert1);
	free(insert2);
	free(insert3);
	free(begin);
	free(insert4);
	free(insert5);
	free(insert6);
	free(insert7);
	free(insert8);
	free(bst);

	return 0;
}

static int CounterFuncIMP(void *data ,void *counter)
{
	UNUSE(data);	

	*((size_t *)counter) += 1;

	return 0;
}

int BSTNextTest()
{
	int num1 = 8;

	int num2 = 3;
	int num3 = 10;

	int num4 = 1;
	int num5 = 6;
	int num6 = 14;
	
	int num7 = 4;
	int num8 = 7;
	int num9 = 13;

	bst_iter_t insert1 = NULL;
	bst_iter_t insert2 = NULL;
	bst_iter_t insert3 = NULL;
	bst_iter_t insert4 = NULL;
	bst_iter_t insert5 = NULL;
	bst_iter_t insert6 = NULL;
	bst_iter_t insert7 = NULL;
	bst_iter_t insert8 = NULL;
	bst_iter_t insert9 = NULL;

	bst_t *bst = BSTCreate(ComparisonFunc, NULL);
	
	insert1 = BSTInsert(bst, &num1);
	insert2 = BSTInsert(bst, &num2);
	insert3 = BSTInsert(bst, &num3);
	insert4 = BSTInsert(bst, &num4);
	insert5 = BSTInsert(bst, &num5);
	insert6 = BSTInsert(bst, &num6);
	insert7 = BSTInsert(bst, &num7);
	insert8 = BSTInsert(bst, &num8);
	insert9 = BSTInsert(bst, &num9);
	
	RUN_TEST("insert4 next insert2", insert2 == BSTNext(insert4));
	RUN_TEST("insert2 next insert7", insert7 == BSTNext(insert2));
	RUN_TEST("insert7 next insert5", insert5 == BSTNext(insert7));
	RUN_TEST("insert5 next insert8", insert8 == BSTNext(insert5));
	RUN_TEST("insert8 next insert1", insert1 == BSTNext(insert8));
	RUN_TEST("insert1 next insert3", insert3 == BSTNext(insert1));
	RUN_TEST("insert3 next insert9", insert9 == BSTNext(insert3));
	RUN_TEST("insert9 next insert6", insert6 == BSTNext(insert9));
	RUN_TEST("insert6 next stub", BSTEnd(bst) == BSTNext(insert6));
	
	free(insert1);
	free(insert2);
	free(insert3);
	free(insert4);
	free(insert5);
	free(insert6);
	free(insert7);
	free(insert8);
	free(insert9);
	free(bst);

	return 0;
}

int BSTPrevTest()
{
	int num1 = 8;

	int num2 = 3;
	int num3 = 10;

	int num4 = 1;
	int num5 = 6;
	int num6 = 14;
	
	int num7 = 4;
	int num8 = 7;
	int num9 = 13;

	bst_iter_t insert1 = NULL;
	bst_iter_t insert2 = NULL;
	bst_iter_t insert3 = NULL;
	bst_iter_t insert4 = NULL;
	bst_iter_t insert5 = NULL;
	bst_iter_t insert6 = NULL;
	bst_iter_t insert7 = NULL;
	bst_iter_t insert8 = NULL;
	bst_iter_t insert9 = NULL;

	bst_t *bst = BSTCreate(ComparisonFunc, NULL);
	
	insert1 = BSTInsert(bst, &num1);
	insert2 = BSTInsert(bst, &num2);
	insert3 = BSTInsert(bst, &num3);
	insert4 = BSTInsert(bst, &num4);
	insert5 = BSTInsert(bst, &num5);
	insert6 = BSTInsert(bst, &num6);
	insert7 = BSTInsert(bst, &num7);
	insert8 = BSTInsert(bst, &num8);
	insert9 = BSTInsert(bst, &num9);
	
	RUN_TEST("end prev insert6", insert6 == BSTPrev(BSTEnd(bst)));
	RUN_TEST("insert6 prev insert9", insert9 == BSTPrev(insert6));
	RUN_TEST("insert9 prev insert3", insert3 == BSTPrev(insert9));
	RUN_TEST("insert3 prev insert1", insert1 == BSTPrev(insert3));
	RUN_TEST("insert1 prev insert8", insert8 == BSTPrev(insert1));
	RUN_TEST("insert8 prev insert5", insert5 == BSTPrev(insert8));
	RUN_TEST("insert5 prev insert7", insert7 == BSTPrev(insert5));
	RUN_TEST("insert7 prev insert2", insert2 == BSTPrev(insert7));
	RUN_TEST("insert2 prev insert4", insert4 == BSTPrev(insert2));

	
	free(insert1);
	free(insert2);
	free(insert3);
	free(insert4);
	free(insert5);
	free(insert6);
	free(insert7);
	free(insert8);
	free(insert9);
	free(bst);

	return 0;
}

int BSTBeginTest()
{
	int num1 = 8;

	int num2 = 3;
	int num3 = 10;

	int num4 = 1;
	int num5 = 6;
	int num6 = 14;
	
	int num7 = 4;
	int num8 = 7;
	int num9 = 13;

	bst_iter_t insert1 = NULL;
	bst_iter_t insert2 = NULL;
	bst_iter_t insert3 = NULL;
	bst_iter_t insert4 = NULL;
	bst_iter_t insert5 = NULL;
	bst_iter_t insert6 = NULL;
	bst_iter_t insert7 = NULL;
	bst_iter_t insert8 = NULL;
	bst_iter_t insert9 = NULL;

	bst_t *bst = BSTCreate(ComparisonFunc, NULL);

	RUN_TEST("begin_inclusive_at_empty", BSTEnd(bst) == BSTBegin(bst));
	insert1 = BSTInsert(bst, &num1);
	RUN_TEST("begin_inclusive_at_insert1", num1 == *((int *)BSTGetData(BSTBegin(bst))));
	insert2 = BSTInsert(bst, &num2);
	RUN_TEST("begin_inclusive_at_insert2", num2 == *((int *)BSTGetData(BSTBegin(bst))));
	insert3 = BSTInsert(bst, &num3);
	RUN_TEST("begin_inclusive_at_insert3", num2 == *((int *)BSTGetData(BSTBegin(bst))));
	insert4 = BSTInsert(bst, &num4);
	RUN_TEST("begin_inclusive_at_insert4", num4 == *((int *)BSTGetData(BSTBegin(bst))));
	insert5 = BSTInsert(bst, &num5);
	RUN_TEST("begin_inclusive_at_insert5", num4 == *((int *)BSTGetData(BSTBegin(bst))));
	insert6 = BSTInsert(bst, &num6);
	RUN_TEST("begin_inclusive_at_insert6", num4 == *((int *)BSTGetData(BSTBegin(bst))));
	insert7 = BSTInsert(bst, &num7);
	RUN_TEST("begin_inclusive_at_insert7", num4 == *((int *)BSTGetData(BSTBegin(bst))));
	insert8 = BSTInsert(bst, &num8);
	RUN_TEST("begin_inclusive_at_insert8", num4 == *((int *)BSTGetData(BSTBegin(bst))));
	insert9 = BSTInsert(bst, &num9);
	RUN_TEST("begin_inclusive_at_insert9", num4 == *((int *)BSTGetData(BSTBegin(bst))));

	free(insert1);
	free(insert2);
	free(insert3);
	free(insert4);
	free(insert5);
	free(insert6);
	free(insert7);
	free(insert8);
	free(insert9);
	free(bst);

	return 0;
}

#ifdef WHITE_BOX_TEST
int BSTEndTest()
{
	bst_t *bst = BSTCreate(ComparisonFunc, NULL);
	
	RUN_TEST("end_exclusive (stub)", &(bst->stub) == BSTEnd(bst));

	free(bst);

	return 0;
}
#endif

int BSTGetDataTest()
{
	int num1 = 8;
	bst_iter_t insert1 = NULL;		

	bst_t *bst = BSTCreate(ComparisonFunc, NULL);
	
	insert1 = BSTInsert(bst, &num1);
	RUN_TEST("is_same_data", 8 == *(int *)BSTGetData(insert1));

	free(insert1);
	free(bst);

	return 0;
}

int BSTIsSameIteratorTest()
{	
	int num1 = 8;
	bst_iter_t insert1 = NULL;		

	bst_t *bst = BSTCreate(ComparisonFunc, NULL);
	
	insert1 = BSTInsert(bst, &num1);
	RUN_TEST("is_begin_same_iter", insert1 == BSTBegin(bst));

	free(insert1);
	free(bst);

	return 0;
}
static int ComparisonFunc(const void *new_data, 
					 	  const void *src_data, 
					  	  const void *comp_param)
{
	UNUSE(comp_param);	

	if(NULL == src_data)
	{
		return -1;
	}

	if(*((int *)new_data) > *((int *)src_data))
	{
		return 1;
	}
	else if(*((int *)new_data) < *((int *)src_data))
	{
		return -1;
	}

	return 0;
}
