/**********************************************************************
 *	This code is representing my tests of AVL 			              *
 *	Lab: OL-79                                		                  *
 *	Author: Yonatan Vologdin 										  *
 *	Reviewer: Tamir Dayan	 	  							     	  *
 *	Date: 25.12.19													  *
 **********************************************************************/

#include <stdio.h>	/*printf*/
#include <stdlib.h>	/*malloc ,free*/
#include <assert.h>	/*assert*/

#include "avl.h"
#include "yoni_utils.h"

/*Uncomment the line below in order to activate white-box tests*/
#define WHITE_BOX_TEST

int AVLCreateTest();
int AVLDestroyTest();
int AVLInsertTest();
int AVLRemoveTest();
int AVLGetHeightTest();
int AVLFindTest();
int AVLIsEmptyTest();
int AVLSizeTest();
int AVLForEachTest();
int LeftLeftBalanceInsertTest();
int RightRightBalanceInsertTest();
int LeftRightBalanceInsertTest();
int RightLeftBalanceInsertTest();

static int ComparisonFunc(const void *new_data, const void *src_data);

#ifdef WHITE_BOX_TEST
typedef struct avl_node avl_node_t;
struct avl_node
{
	avl_node_t *children[2];
	void *data;
	size_t height; 
};

struct avl
{
	comparison_func_t comparison_func;
	avl_node_t *root;
};
#endif

int main()
{
	RUN_SEQUENCE("AVLCreateTest", AVLCreateTest());
	RUN_SEQUENCE("AVLDestroyTest", AVLDestroyTest());
	RUN_SEQUENCE("AVLInsertTest", AVLInsertTest());
	RUN_SEQUENCE("AVLRemoveTest", AVLRemoveTest());
	RUN_SEQUENCE("AVLGetHeightTest", AVLGetHeightTest());
	RUN_SEQUENCE("AVLFindTest", AVLFindTest());
	RUN_SEQUENCE("AVLIsEmptyTest", AVLIsEmptyTest());
	RUN_SEQUENCE("AVLSizeTest", AVLSizeTest());
	RUN_SEQUENCE("AVLForEachTest", AVLForEachTest());
	RUN_SEQUENCE("LeftLeftBalanceInsertTest", LeftLeftBalanceInsertTest());
	RUN_SEQUENCE("RightRightBalanceInsertTest", RightRightBalanceInsertTest());
	RUN_SEQUENCE("LeftRightBalanceInsertTest", LeftRightBalanceInsertTest());
	RUN_SEQUENCE("RightLeftBalanceInsertTest", RightLeftBalanceInsertTest());

	return 0;
}

int AVLCreateTest()
{
	avl_t *tree = AVLCreate(ComparisonFunc);

	printf("----Empty Test----\n");

	AVLDestroy(tree);
	
	return 0;
}

int AVLDestroyTest()
{
	avl_t *tree = AVLCreate(ComparisonFunc);

	printf("----Empty Test----\n");

	AVLDestroy(tree);
	
	return 0;
}

int AVLInsertTest()
{
	int insert1 = 8;
	int insert2 = 10;
	int insert3 = 4;
	int insert4 = 1;
	int insert5 = 9;
	int insert6 = 11;
	int insert7 = 5;

	avl_t *tree = AVLCreate(ComparisonFunc);
	assert(tree);
	
	AVLInsert(tree, &insert1);
	AVLInsert(tree, &insert2);
	AVLInsert(tree, &insert3);
	AVLInsert(tree, &insert4);
	AVLInsert(tree, &insert5);
	AVLInsert(tree, &insert6);
	AVLInsert(tree, &insert7);

#ifdef WHITE_BOX_TEST
	RUN_TEST("wb insert1", insert1 == *(int *)tree->root->data);
	RUN_TEST("wb insert2", insert2 == *(int *)tree->root->children[1]->data);
	RUN_TEST("wb insert3", insert3 == *(int *)tree->root->children[0]->data);
	RUN_TEST("wb insert4", insert4 == *(int *)tree->root->children[0]->children[0]->data);
	RUN_TEST("wb insert5", insert5 == *(int *)tree->root->children[1]->children[0]->data);
	RUN_TEST("wb insert6", insert6 == *(int *)tree->root->children[1]->children[1]->data);
	RUN_TEST("wb insert7", insert7 == *(int *)tree->root->children[0]->children[1]->data);
#endif

	printf("\n----Check Valgrind----\n");

	AVLDestroy(tree);
	
	return 0;
}

int AVLRemoveTest()
{
	int insert1 = 8;
	int insert2 = 10;
	int insert3 = 4;
	int insert4 = 1;
	int insert5 = 9;
	int insert6 = 11;
	int insert7 = 5;

	avl_t *tree = AVLCreate(ComparisonFunc);
	assert(tree);
	
	AVLInsert(tree, &insert1);
	AVLInsert(tree, &insert2);
	AVLInsert(tree, &insert3);
	AVLInsert(tree, &insert4);
	AVLInsert(tree, &insert5);
	AVLInsert(tree, &insert6);
	AVLInsert(tree, &insert7);

	AVLRemove(tree, &insert6);
	RUN_TEST("find 6", NULL == AVLFind(tree, &insert6));
	RUN_TEST("size 6", 6 == AVLSize(tree));
	RUN_TEST("height", 2 == AVLGetHeight(tree));
#ifdef WHITE_BOX_TEST
	RUN_TEST("wb remove 6 (leaf)", 
	9 == *(int *)tree->root->children[1]->children[0]->data);
#endif

	AVLRemove(tree, &insert2);
	RUN_TEST("find 2", NULL == AVLFind(tree, &insert6));
	RUN_TEST("size 2", 5 == AVLSize(tree));
	RUN_TEST("height", 2 == AVLGetHeight(tree));
#ifdef WHITE_BOX_TEST
	RUN_TEST("wb remove 2 (right child, has left child)", 
	9 == *(int *)tree->root->children[1]->data);
#endif

	AVLRemove(tree, &insert3);
	RUN_TEST("find 3", NULL == AVLFind(tree, &insert6));
	RUN_TEST("size 3", 4 == AVLSize(tree));
	RUN_TEST("height", 2 == AVLGetHeight(tree));
#ifdef WHITE_BOX_TEST
	RUN_TEST("wb remove 3 (right child, has left child)", 
	1 == *(int *)tree->root->children[0]->children[0]->data);
#endif

	AVLRemove(tree, &insert1);
	RUN_TEST("find 1", NULL == AVLFind(tree, &insert6));
	RUN_TEST("size 1", 3 == AVLSize(tree));
	RUN_TEST("height", 2 == AVLGetHeight(tree));
#ifdef WHITE_BOX_TEST
	RUN_TEST("wb remove 1 (right child, has left child)", 
	9 == *(int *)tree->root->data);
#endif

	AVLRemove(tree, &insert4);
	RUN_TEST("find 4", NULL == AVLFind(tree, &insert6));
	RUN_TEST("size 4", 2 == AVLSize(tree));
	RUN_TEST("height", 1 == AVLGetHeight(tree));

	AVLRemove(tree, &insert5);
	RUN_TEST("find 5", NULL == AVLFind(tree, &insert6));
	RUN_TEST("size 5", 1 == AVLSize(tree));
	RUN_TEST("height(only root left)", 0 == AVLGetHeight(tree));

	AVLRemove(tree, &insert7);
	RUN_TEST("find 7", NULL == AVLFind(tree, &insert6));
	RUN_TEST("size 7", 0 == AVLSize(tree));
	/*now tree is empty*/

	AVLDestroy(tree);
	
	return 0;
}

int AVLGetHeightTest()
{
	int insert1 = 8;
	int insert2 = 10;
	int insert3 = 4;
	int insert4 = 1;
	int insert5 = 9;
	int insert6 = 11;
	int insert7 = 5;
	int insert8 = 6;

	avl_t *tree = AVLCreate(ComparisonFunc);
	assert(tree);
	
	AVLInsert(tree, &insert1);
	RUN_TEST("insert1(root)", 0 == AVLGetHeight(tree));
	AVLInsert(tree, &insert2);
	RUN_TEST("insert2", 1 == AVLGetHeight(tree));
	AVLInsert(tree, &insert3);
	RUN_TEST("insert3", 1 == AVLGetHeight(tree));
	AVLInsert(tree, &insert4);
	RUN_TEST("insert4", 2 == AVLGetHeight(tree));
	AVLInsert(tree, &insert5);
	RUN_TEST("insert5", 2 == AVLGetHeight(tree));
	AVLInsert(tree, &insert6);
	RUN_TEST("insert6", 2 == AVLGetHeight(tree));
	AVLInsert(tree, &insert7);
	RUN_TEST("insert7", 2 == AVLGetHeight(tree));
	AVLInsert(tree, &insert8);
	RUN_TEST("insert8", 3 == AVLGetHeight(tree));

	AVLDestroy(tree);
	
	return 0;
}

int AVLFindTest()
{
	int insert1 = 8;
	int insert2 = 10;
	int insert3 = 4;
	int insert4 = 1;
	int insert5 = 9;
	int insert6 = 11;
	int insert7 = 5;
	int non_exist = 99;

	avl_t *tree = AVLCreate(ComparisonFunc);
	assert(tree);
	
	AVLInsert(tree, &insert1);
	AVLInsert(tree, &insert2);
	AVLInsert(tree, &insert3);
	AVLInsert(tree, &insert4);
	AVLInsert(tree, &insert5);
	AVLInsert(tree, &insert6);
	AVLInsert(tree, &insert7);

	RUN_TEST("insert1", &insert1 == AVLFind(tree, &insert1));
	RUN_TEST("insert2", &insert2 == AVLFind(tree, &insert2));
	RUN_TEST("insert3", &insert3 == AVLFind(tree, &insert3));
	RUN_TEST("insert4", &insert4 == AVLFind(tree, &insert4));
	RUN_TEST("insert5", &insert5 == AVLFind(tree, &insert5));
	RUN_TEST("insert6", &insert6 == AVLFind(tree, &insert6));
	RUN_TEST("insert7", &insert7 == AVLFind(tree, &insert7));
	RUN_TEST("non_exist", NULL == AVLFind(tree, &non_exist));
	
	AVLDestroy(tree);
	
	return 0;
}

int AVLIsEmptyTest()
{
	int insert1 = 8;

	avl_t *tree = AVLCreate(ComparisonFunc);
	assert(tree);
	
	RUN_TEST("no insert", 1 == AVLIsEmpty(tree));
	AVLInsert(tree, &insert1);
	RUN_TEST("one insert", 0 == AVLIsEmpty(tree));
	
	AVLDestroy(tree);
	
	return 0;
}

int AVLSizeTest()
{
	int insert1 = 8;
	int insert2 = 10;
	int insert3 = 4;
	int insert4 = 1;
	int insert5 = 9;
	int insert6 = 11;
	int insert7 = 5;

	avl_t *tree = AVLCreate(ComparisonFunc);
	assert(tree);
	
	RUN_TEST("empty tree", 0 == AVLSize(tree));
	AVLInsert(tree, &insert1);
	RUN_TEST("insert1", 1 == AVLSize(tree));
	AVLInsert(tree, &insert2);
	RUN_TEST("insert2", 2 == AVLSize(tree));
	AVLInsert(tree, &insert3);
	RUN_TEST("insert3", 3 == AVLSize(tree));
	AVLInsert(tree, &insert4);
	RUN_TEST("insert4", 4 == AVLSize(tree));
	AVLInsert(tree, &insert5);
	RUN_TEST("insert5", 5 == AVLSize(tree));
	AVLInsert(tree, &insert6);
	RUN_TEST("insert6", 6 == AVLSize(tree));
	AVLInsert(tree, &insert7);
	RUN_TEST("insert7", 7 == AVLSize(tree));

	AVLDestroy(tree);
	
	return 0;
}

static int AddOne(void *data, void *for_each_param)
{
	UNUSE(for_each_param);
	
	*(int *)data += 1;

	return 0;
}

static int CounterFunc(void *data, void *for_each_param)
{
	UNUSE(data);
	
	*(size_t *)for_each_param += 1;

	return 0;
}

int AVLForEachTest()
{
	int insert1 = 8;
	int insert2 = 10;
	int insert3 = 4;
	int insert4 = 1;
	int insert5 = 9;
	int insert6 = 11;
	int insert7 = 5;

	int add_value = 1;	

	size_t counter = 0;

	avl_t *tree = AVLCreate(ComparisonFunc);
	assert(tree);
	
	AVLInsert(tree, &insert1);
	AVLInsert(tree, &insert2);
	AVLInsert(tree, &insert3);
	AVLInsert(tree, &insert4);
	AVLInsert(tree, &insert5);
	AVLInsert(tree, &insert6);
	AVLInsert(tree, &insert7);

	AVLForEach(tree, CounterFunc, &counter);
	RUN_TEST("counter\t\t", 7 == counter);

	AVLForEach(tree, AddOne, &add_value);
	RUN_TEST("updated insert1", 9 == insert1);
	RUN_TEST("updated insert2", 11 == insert2);
	RUN_TEST("updated insert3", 5 == insert3);
	RUN_TEST("updated insert4", 2 == insert4);
	RUN_TEST("updated insert5", 10 == insert5);
	RUN_TEST("updated insert6", 12 == insert6);
	RUN_TEST("updated insert7", 6 == insert7);

	AVLDestroy(tree);
	
	return 0;
}

int LeftLeftBalanceInsertTest()
{
	int insert1 = 3;
	int insert2 = 2;
	int insert3 = 1;

	avl_t *tree = AVLCreate(ComparisonFunc);
	assert(tree);
	
	AVLInsert(tree, &insert1);
	AVLInsert(tree, &insert2);
	AVLInsert(tree, &insert3);

#ifdef WHITE_BOX_TEST
	RUN_TEST("left_left wb place", insert2 == *(int *)tree->root->data);
	RUN_TEST("left_left wb place", insert3 == *(int *)tree->root->children[0]->data);
	RUN_TEST("left_left wb place", insert1 == *(int *)tree->root->children[1]->data);

	RUN_TEST("left_left wb NULL", NULL == tree->root->children[0]->children[0]);
	RUN_TEST("left_left wb NULL", NULL == tree->root->children[0]->children[1]);
	RUN_TEST("left_left wb NULL", NULL == tree->root->children[1]->children[0]);
	RUN_TEST("left_left wb NULL", NULL == tree->root->children[1]->children[1]);

	RUN_TEST("left_left wb height", 1 == (size_t)tree->root->height);
	RUN_TEST("left_left wb height", 0 == (size_t)tree->root->children[0]->height);
	RUN_TEST("left_left wb height", 0 == (size_t)tree->root->children[1]->height);
#endif

	AVLDestroy(tree);
	
	return 0;
}

int RightRightBalanceInsertTest()
{
	int insert1 = 1;
	int insert2 = 2;
	int insert3 = 3;

	avl_t *tree = AVLCreate(ComparisonFunc);
	assert(tree);
	
	AVLInsert(tree, &insert1);
	AVLInsert(tree, &insert2);
	AVLInsert(tree, &insert3);

#ifdef WHITE_BOX_TEST
	RUN_TEST("right_right wb place", insert2 == *(int *)tree->root->data);
	RUN_TEST("right_right wb place", insert1 == *(int *)tree->root->children[0]->data);
	RUN_TEST("right_right wb place", insert3 == *(int *)tree->root->children[1]->data);

	RUN_TEST("right_right wb NULL", NULL == tree->root->children[0]->children[0]);
	RUN_TEST("right_right wb NULL", NULL == tree->root->children[0]->children[1]);
	RUN_TEST("right_right wb NULL", NULL == tree->root->children[1]->children[0]);
	RUN_TEST("right_right wb NULL", NULL == tree->root->children[1]->children[1]);

	RUN_TEST("right_right wb height", 1 == (size_t)tree->root->height);
	RUN_TEST("right_right wb height", 0 == (size_t)tree->root->children[0]->height);
	RUN_TEST("right_right wb height", 0 == (size_t)tree->root->children[1]->height);
#endif

	AVLDestroy(tree);
	
	return 0;
}

int LeftRightBalanceInsertTest()
{
	int insert1 = 5;
	int insert2 = 2;
	int insert3 = 3;

	avl_t *tree = AVLCreate(ComparisonFunc);
	assert(tree);
	
	AVLInsert(tree, &insert1);
	AVLInsert(tree, &insert2);
	AVLInsert(tree, &insert3);

#ifdef WHITE_BOX_TEST
	RUN_TEST("left_right wb place", insert3 == *(int *)tree->root->data);
	RUN_TEST("left_right wb place", insert2 == *(int *)tree->root->children[0]->data);
	RUN_TEST("left_right wb place", insert1 == *(int *)tree->root->children[1]->data);

	RUN_TEST("left_right wb NULL", NULL == tree->root->children[0]->children[0]);
	RUN_TEST("left_right wb NULL", NULL == tree->root->children[0]->children[1]);
	RUN_TEST("left_right wb NULL", NULL == tree->root->children[1]->children[0]);
	RUN_TEST("left_right wb NULL", NULL == tree->root->children[1]->children[1]);

	RUN_TEST("left_right wb height", 1 == (size_t)tree->root->height);
	RUN_TEST("left_right wb height", 0 == (size_t)tree->root->children[0]->height);
	RUN_TEST("left_right wb height", 0 == (size_t)tree->root->children[1]->height);
#endif

	AVLDestroy(tree);
	
	return 0;
}

int RightLeftBalanceInsertTest()
{
	int insert1 = 2;
	int insert2 = 5;
	int insert3 = 3;

	avl_t *tree = AVLCreate(ComparisonFunc);
	assert(tree);
	
	AVLInsert(tree, &insert1);
	AVLInsert(tree, &insert2);
	AVLInsert(tree, &insert3);

#ifdef WHITE_BOX_TEST
	RUN_TEST("right_left wb place", insert3 == *(int *)tree->root->data);
	RUN_TEST("right_left wb place", insert1 == *(int *)tree->root->children[0]->data);
	RUN_TEST("right_left wb place", insert2 == *(int *)tree->root->children[1]->data);

	RUN_TEST("right_left wb NULL", NULL == tree->root->children[0]->children[0]);
	RUN_TEST("right_left wb NULL", NULL == tree->root->children[0]->children[1]);
	RUN_TEST("right_left wb NULL", NULL == tree->root->children[1]->children[0]);
	RUN_TEST("right_left wb NULL", NULL == tree->root->children[1]->children[1]);

	RUN_TEST("right_left wb height", 1 == (size_t)tree->root->height);
	RUN_TEST("right_left wb height", 0 == (size_t)tree->root->children[0]->height);
	RUN_TEST("right_left wb height", 0 == (size_t)tree->root->children[1]->height);
#endif

	AVLDestroy(tree);
	
	return 0;
}

static int ComparisonFunc(const void *new_data, const void *src_data)
{
	if (*(int *)new_data > *(int *)src_data)
	{
		return (1);
	}
	else if (*(int *)new_data < *(int *)src_data)
	{
		return (-1);
	}

	return (0);
}
