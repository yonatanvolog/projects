/**********************************************************************
 *	This code is representing my functions of BST RC1		          *
 *	Lab: OL-79                                		                  *
 *	Author: Yonatan Vologdin 										  *
 *	Reviewer: Sharon Rottner	 	  								  *
 *	Date: 16.12.19													  *
 **********************************************************************/

#include <stdlib.h>		/*malloc*/
#include <assert.h>		/*assert*/

#include "bst.h"		/*bst*/
#include "yoni_utils.h"	/*utils*/

#define YES 1
#define NO -1
#define EQUAL 0

#define LEFT 0
#define RIGHT 1

#define PREV 0
#define NEXT 1

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

static bst_node_t *CreateNodeIMP(void *data);
static int NodeIsLeafIMP(bst_node_t *node);
static int CounterFuncIMP(void *data ,void *counter);
static int HasChildIMP(bst_iter_t *node, int direction);
static void GoToChildIMP(bst_iter_t *node, int direction);
static int IsChildIMP(bst_iter_t *node, int direction);
static bst_iter_t GoToDirectionIMP(bst_iter_t *iter, int direction);
static void SetChildForParentIMP(bst_iter_t iter, int direction,
 												  bst_iter_t set_to);
static void SetParentForChildIMP(bst_iter_t iter, bst_iter_t set_to);
static bst_iter_t GetOnlyChildIMP(bst_iter_t iter);
static int HasOnlyOneChildIMP(bst_iter_t iter);
static void RemoveNodeIMP(bst_iter_t iter);

static bst_iter_t GetRootIMP(bst_t *tree);
static bst_iter_t GetStubIMP(bst_t *tree);
static bst_iter_t GetParentOfNewNodeIMP(bst_iter_t runner_node,
										void *data,
										comparison_func comp_func,
										void *comp_param);
static void InsertNewNodeIMP(bst_iter_t new_node,
						     bst_iter_t parent_node,
						     void *data,
						     comparison_func comp_func,
						     void *comp_param);

bst_t *BSTCreate(comparison_func func, void *param)
{
	bst_node_t stub = {0};
	bst_t *new_bst = (bst_t *)malloc(sizeof(bst_t));
	if (NULL == new_bst)
	{
		return (NULL);
	}

	assert(func);

	(new_bst->stub).parent = NULL;
	(new_bst->stub).left = NULL;
	(new_bst->stub).right = NULL;
	(new_bst->stub).data = NULL;
	
	new_bst->comparison_func_ptr = func;
	new_bst->comparison_param = param;
	new_bst->stub = stub;

	return (new_bst);
}

void BSTDestroy(bst_t *tree)
{
	bst_iter_t runner = NULL;
	bst_iter_t leaf_to_remove = NULL;
	
	assert(tree);

	runner = GetRootIMP(tree);

	while(!BSTIsEmpty(tree))
	{
		if(NodeIsLeafIMP(runner))
		{
			leaf_to_remove = runner;
			runner = runner->parent;
			
			if(IsChildIMP(&leaf_to_remove, LEFT))
			{
				runner->left = NULL;
			}
			else	/*node is right child*/
			{
				runner->right = NULL;
			}

			free(leaf_to_remove);
		}

		else if(HasChildIMP(&runner, LEFT))
		{
			GoToChildIMP(&runner, LEFT);
		}

		else	/*node has right child*/
		{
			GoToChildIMP(&runner, RIGHT);
		}
	}
	free(tree);
}

bst_iter_t BSTInsert(bst_t *tree, void *data)
{
	comparison_func comp_func = NULL;
	void *comp_param = NULL;
	bst_iter_t parent_node = NULL;
	bst_iter_t runner_node = NULL;

	bst_iter_t new_node = CreateNodeIMP(data);
	if(NULL == new_node)
	{
		return (BSTEnd(tree));
	}

	assert(tree);

	comp_func = tree->comparison_func_ptr;
	comp_param = tree->comparison_param;
	parent_node = GetStubIMP(tree);
	runner_node = parent_node->left;
	
	if(BSTIsEmpty(tree))
	{
		parent_node->left = new_node;
		new_node->parent = parent_node;

		return (new_node);
	}

	parent_node = GetParentOfNewNodeIMP(runner_node,
									 	data, 
										comp_func, 
										comp_param);
	InsertNewNodeIMP(new_node,
					 parent_node,
				 	 data, 
					 comp_func, 
					 comp_param);

	return (new_node);
}

void BSTRemove(bst_iter_t iter)
{
	bst_iter_t new_child = iter->right;
	bst_iter_t new_parent_for_left = iter;
	int my_direction = RIGHT;

	if(IsChildIMP(&iter, LEFT))
	{
		my_direction = LEFT;
	}

	if(NodeIsLeafIMP(iter))
	{
		SetChildForParentIMP(iter->parent, my_direction, NULL);
	}

	else if(HasOnlyOneChildIMP(iter))
	{
		SetChildForParentIMP(iter->parent, my_direction, GetOnlyChildIMP(iter));
		SetParentForChildIMP(GetOnlyChildIMP(iter), (iter->parent));
	}

	else	/*node has 2 children*/
	{
		SetChildForParentIMP(iter->parent, my_direction, iter->right);
		SetParentForChildIMP(iter->right, (iter->parent));

		new_parent_for_left = new_child;
		while(NULL != new_parent_for_left->left)
		{
			new_parent_for_left = new_parent_for_left->left;
		}
		
		SetChildForParentIMP(new_parent_for_left, LEFT, iter->left);
		SetParentForChildIMP(iter->left, new_parent_for_left);
	}

	RemoveNodeIMP(iter);
}

bst_iter_t BSTFind(bst_t *tree, void *data)
{
	comparison_func comp_func = NULL;
	void *comp_param = NULL;
	bst_iter_t runner_node = NULL;
	int is_new_data_larger = 0;

	assert(tree);

	comp_func = tree->comparison_func_ptr;
	comp_param = tree->comparison_param;
	runner_node = GetRootIMP(tree);
	
	while(NULL != runner_node)
	{
		is_new_data_larger = comp_func(data,
									   BSTGetData(runner_node),
									   comp_param);
				
		if(EQUAL == is_new_data_larger)
		{
			return (runner_node);
		}

		else if(YES == is_new_data_larger)
		{
			GoToChildIMP(&runner_node, RIGHT);
		}
	
		else
		{
			GoToChildIMP(&runner_node, LEFT);
		}
	}
	
	return (BSTEnd(tree));
}

int BSTIsEmpty(const bst_t *tree)
{		
	assert(tree);
	
	return (NULL == (tree->stub).left);
}

size_t BSTSize(const bst_t *tree)
{
	size_t counter = 0;
	
	assert(tree);

	BSTForEach(BSTBegin(tree), BSTEnd(tree), CounterFuncIMP, &counter);

	return (counter);
}
	
static int CounterFuncIMP(void *data ,void *counter)
{
	UNUSE(data);	

	*((size_t *)counter) += 1;

	return (0);
}

int BSTForEach(bst_iter_t begin, bst_iter_t end, action_func func, void *param)
{
	int action_return_value = 0;
	bst_iter_t runner = begin;

	assert(begin);
	assert(end);
	assert(func);
	
	while((!BSTIsSameIterator(runner,end)) && (0 == action_return_value))
	{
		action_return_value = func(runner->data, param);
		runner = BSTNext(runner);
	}

	return (action_return_value);
}

bst_iter_t BSTNext(bst_iter_t iter)
{
	return (GoToDirectionIMP(&iter, NEXT));
}

bst_iter_t BSTPrev(bst_iter_t iter)
{
	return (GoToDirectionIMP(&iter, PREV));
}

bst_iter_t BSTBegin(const bst_t *tree)
{
	bst_iter_t parent_node = NULL;
	bst_iter_t runner_node = NULL;
	
	assert(tree);	

	parent_node = GetStubIMP((bst_t *)tree);
	runner_node = parent_node->left;	

	while(NULL != runner_node)
	{
		parent_node = runner_node;
		GoToChildIMP(&runner_node, LEFT);
	}
	
	return (parent_node);
}

bst_iter_t BSTEnd(const bst_t *tree)
{
	assert(tree);
	
	return (GetStubIMP((bst_t *)tree));
}

void *BSTGetData(bst_iter_t iter)
{	
	return (iter->data);
}

int BSTIsSameIterator(bst_iter_t iter1, bst_iter_t iter2)
{
	return (iter1 == iter2);
}

static bst_iter_t GoToDirectionIMP(bst_iter_t *iter, int direction)
{
	int direction1 = RIGHT;
	int direction2 = LEFT;
	
	if(PREV == direction)
	{
		direction1 = LEFT;
		direction2 = RIGHT;
	}

	if(HasChildIMP(iter, direction1))
	{
		GoToChildIMP(iter, direction1);
		while(HasChildIMP(iter, direction2))
		{
			GoToChildIMP(iter, direction2);
		}
	}
	else if(IsChildIMP(iter, direction2))
	{
		*iter = (*iter)->parent;
	}
	else if(IsChildIMP(iter, direction1))
	{
		while(IsChildIMP(&((*iter)->parent), direction1))
		{
			*iter = (*iter)->parent;
		}
		*iter = ((*iter)->parent)->parent;
	}
	
	return (*iter);
}

static int HasChildIMP(bst_iter_t *node, int direction)
{
	if(LEFT == direction)
	{
		return (NULL != (*node)->left);
	}
	else
	{
		return (NULL != (*node)->right);
	}
}

static void GoToChildIMP(bst_iter_t *node, int direction)
{
	if(LEFT == direction)
	{
		(*node) = (*node)->left;
		

	}
	else
	{
		(*node) = (*node)->right;
	}
}

static int IsChildIMP(bst_iter_t *node, int direction)
{	
	if(LEFT == direction)
	{
		return ((*node) == (((*node)->parent)->left));
	}
	else
	{
		return ((*node) == (((*node)->parent)->right));
	}
}

static void SetChildForParentIMP(bst_iter_t iter, int direction, 
												   bst_iter_t set_to)
{
	if(LEFT == direction)
	{
		iter->left = set_to;
	}
	else
	{
		iter->right = set_to;
	}	
}

static void SetParentForChildIMP(bst_iter_t iter, bst_iter_t set_to)
{
	iter->parent = set_to;
}

static bst_iter_t GetOnlyChildIMP(bst_iter_t iter)
{
	if(HasChildIMP(&iter, LEFT))
	{
		return (iter->left);
	}

	return (iter->right);
}

static int HasOnlyOneChildIMP(bst_iter_t iter)
{
	return (((HasChildIMP(&iter, LEFT)) && (!HasChildIMP(&iter, RIGHT))) ||
	   	   ((!HasChildIMP(&iter, LEFT)) && (HasChildIMP(&iter, RIGHT))));
}

static void RemoveNodeIMP(bst_iter_t iter)
{
	iter->left = NULL;
	iter->right = NULL;
	iter->parent = NULL;
	iter->data = NULL;

	free(iter);
}

static int NodeIsLeafIMP(bst_node_t *node)
{
	return ((!HasChildIMP(&node, LEFT)) && (!HasChildIMP(&node, RIGHT)));
}

static bst_node_t *CreateNodeIMP(void *data)
{
	bst_node_t *new_node = (bst_node_t *)malloc(sizeof(bst_node_t));
	if(NULL == new_node)
	{
		return (NULL);
	}
	
	new_node->parent = NULL;
	new_node->left = NULL;
	new_node->right = NULL;
	new_node->data = data;

	return (new_node);
}

static bst_iter_t GetParentOfNewNodeIMP(bst_iter_t runner_node,
										void *data,
										comparison_func comp_func,
										void *comp_param)
{
	int is_new_data_larger = NO;
	bst_iter_t parent_node = NULL;

	while(NULL != runner_node)
	{
		is_new_data_larger = comp_func(data,
									   BSTGetData(runner_node),
									   comp_param);
		parent_node = runner_node;		
		if(YES == is_new_data_larger)
		{
			runner_node = runner_node->right;
		}
	
		if(NO == is_new_data_larger)
		{
			GoToChildIMP(&runner_node, LEFT);
		}
	
		assert(EQUAL != is_new_data_larger);
	}

	return parent_node;
}

static void InsertNewNodeIMP(bst_iter_t new_node,
						     bst_iter_t parent_node,
						     void *data,
						     comparison_func comp_func,
						     void *comp_param)
{
	int is_new_data_larger = comp_func(data,
									   BSTGetData(parent_node),
									   comp_param);	
	
	new_node->parent = parent_node;
	if(YES == is_new_data_larger)
	{
		parent_node->right = new_node;
	}
	if(NO == is_new_data_larger)
	{
		parent_node->left = new_node;
	}
	
	assert(EQUAL != is_new_data_larger);
}

static bst_iter_t GetRootIMP(bst_t *tree)
{
	return ((&(tree->stub))->left);
}

static bst_iter_t GetStubIMP(bst_t *tree)
{
	return (&(tree->stub));
}
