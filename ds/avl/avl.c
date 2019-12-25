/**********************************************************************
 *	This code is representing my functions of AVL RC1		          *
 *	Lab: OL-79                                		                  *
 *	Author: Yonatan Vologdin 										  *
 *	Reviewer: Tamir Dayan	 	  							     	  *
 *	Date: 25.12.19													  *
 **********************************************************************/
#include <stdlib.h>	/*malloc, free*/
#include <assert.h>	/*assert*/

#include "avl.h"
#include "yoni_utils.h"

typedef enum direction_e
{
	LEFT = 0,
	EQUAL = 2,
	RIGHT = 1
}direction_e;

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

static avl_node_t *RecInsertIMP(avl_node_t *current_root, void *data, comparison_func_t CmpFunc);
static avl_node_t* CreateNodeIMP(avl_node_t *left_child, avl_node_t *right_child,
														void *data, size_t height);
static direction_e GetDirectionIMP(int cmp_res);
static int IsNodeLeafIMP(avl_node_t *node);
static int IsNodeHasOnlyLeftChildIMP(avl_node_t *node);
static int IsNodeHasOnlyRightChildIMP(avl_node_t *node);

avl_t *AVLCreate(comparison_func_t func)
{
	avl_t *new_avl = (avl_t *)malloc(sizeof(avl_t));
	if(NULL == new_avl)
	{
		return NULL;
	}
	
	new_avl->comparison_func = func;
	new_avl->root = NULL;

	return new_avl;
}

static int IsNodeHasLeftChildIMP(avl_node_t *node)
{
	return (NULL != node->children[LEFT]);
}

static int IsNodeHasRightChildIMP(avl_node_t *node)
{
	return (NULL != node->children[RIGHT]);
}

static int IsNodeLeafIMP(avl_node_t *node)
{
	return ((NULL == node->children[LEFT]) && (NULL == node->children[RIGHT]));
}

static int IsNodeHasOnlyLeftChildIMP(avl_node_t *node)
{
	return ((NULL != node->children[LEFT]) && (NULL == node->children[RIGHT]));
}

static int IsNodeHasOnlyRightChildIMP(avl_node_t *node)
{
	return ((NULL == node->children[LEFT]) && (NULL != node->children[RIGHT]));
}

static int IsNodeHasOnlyOneChildIMP(avl_node_t *node)
{
	return (((NULL == node->children[LEFT]) && (NULL != node->children[RIGHT])) ||
		   (((NULL != node->children[LEFT]) && (NULL == node->children[RIGHT]))));
}

static avl_node_t *GetOnlyChildIMP(avl_node_t *node)
{
	if (1 == IsNodeHasLeftChildIMP(node))
	{
		return (node->children[LEFT]);
	}

	return (node->children[RIGHT]);
}

/*Post-order*/
void RecDestroyIMP(avl_node_t *node)
{
	if (NULL == node)
	{
		return;
	}

	RecDestroyIMP(node->children[LEFT]);
	RecDestroyIMP(node->children[RIGHT]);
	free(node);
}

void AVLDestroy(avl_t *tree)
{
	RecDestroyIMP(tree->root);
	free(tree);
}

int AVLInsert(avl_t *tree, void *data)
{
	avl_node_t *node = tree->root;
	node = RecInsertIMP(node, data, tree->comparison_func);
	if (NULL == node)
	{
		return 1;	
	}
	tree->root = node;

	return 0;
}

static void UpdataHeightIMP(avl_node_t *node)
{
	if (1 == IsNodeLeafIMP(node))
	{
		node->height = 0;
	}
		
	else if(1 == IsNodeHasOnlyLeftChildIMP(node))
	{
		node->height = node->children[LEFT]->height + 1;
	}
	else if(1 == IsNodeHasOnlyRightChildIMP(node))
	{
		node->height = node->children[RIGHT]->height + 1;
	}
	else
	{
		node->height = MAX(node->children[LEFT]->height,
						   node->children[RIGHT]->height) + 1;
	}
}

static avl_node_t *RecInsertIMP(avl_node_t *node, void *data, comparison_func_t CmpFunc)
{
	direction_e direction = 0;
	int cmp_res = 0;

	if (NULL == node)
	{
		node = CreateNodeIMP(NULL, NULL, data, 0);	
		if (NULL == node)
		{
			return NULL;
		}
		
		return node;
	}
	
	cmp_res = CmpFunc(data, node->data);	
	assert(0 != cmp_res);
	direction = GetDirectionIMP(cmp_res);
	
	node->children[direction] =
			RecInsertIMP(node->children[direction], data, CmpFunc);

	UpdataHeightIMP(node);

	return node;
}









/*WORK IN PROGRESS*/
static avl_node_t *RemoveSingleNodeIMP (avl_node_t *node)
{
	avl_node_t *child = NULL;

	if (1 == IsNodeLeafIMP(node))
	{
		free(node);		
	
		return NULL;
	}
	
	else if (1 == IsNodeHasOnlyOneChildIMP(node))
	{
		child = GetOnlyChildIMP(node);
		free(node);
	
		return child;
	}
	
	else /*node has 2 children*/
	{
		
	}
	
	return node;
}

static avl_node_t *RecRemoveIMP(avl_node_t *node, const void *data, comparison_func_t CmpFunc)
{
	direction_e direction = 0;
	int cmp_res = 0;	

	if (NULL == node)
	{
		return NULL;
	}
	
	cmp_res = CmpFunc(data, node->data);
	
	if (0 == cmp_res)
	{
		return RemoveSingleNodeIMP(node);
	}
	
	direction = GetDirectionIMP(cmp_res);
	node->children[direction] = RecRemoveIMP(node->children[direction], data, CmpFunc);

	return node;
}

void AVLRemove(avl_t *tree, const void *data)
{
	tree->root = RecRemoveIMP(tree->root, data, tree->comparison_func);
}















size_t AVLGetHeight(const avl_t *tree)
{
	return (tree->root->height);
}


static avl_node_t *RecFindIMP(avl_node_t *current_root, const void *data, 
										comparison_func_t CmpFunc)
{
	int cmp_res = 0;

	if (NULL == current_root)
	{
		return NULL;
	}

	cmp_res = CmpFunc(data, current_root->data);
	if (0 == cmp_res)
	{
		return current_root;
	}
	else if (1 == cmp_res)
	{
		return RecFindIMP(current_root->children[RIGHT], data, CmpFunc);
	}
	return RecFindIMP(current_root->children[LEFT], data, CmpFunc);
}

void *AVLFind(const avl_t *tree, const void *data)
{
	avl_node_t *return_data = RecFindIMP(tree->root, data, tree->comparison_func);
	if (NULL == return_data)
	{
		return NULL;
	}
	return (return_data->data);
}

int AVLIsEmpty(const avl_t *tree)
{
	return (NULL == tree->root);
}

/*Pre-order*/ 
static void RecSizeIMP(avl_node_t *node, size_t *counter)
{
	if (NULL == node)
	{
		return;
	}
	*counter += 1;
	RecSizeIMP(node->children[LEFT], counter);
	RecSizeIMP(node->children[RIGHT], counter);
}

size_t AVLSize(const avl_t *tree)
{
	size_t counter = 0;
	RecSizeIMP(tree->root, &counter);

	return counter;
}

/*In-order */
static int RecForEachIMP(avl_node_t *node, action_func_t func, void *param)
{
	int func_return_value = 0;	

	if (NULL == node)
	{
		return 0;
	}
	RecForEachIMP(node->children[LEFT], func, param);
	
	func_return_value = func(node->data, param);
	if(0 != func_return_value)
	{
		return func_return_value;
	}

	func_return_value = RecForEachIMP(node->children[RIGHT], func, param);

	return func_return_value;
}


int AVLForEach(avl_t *tree, action_func_t func, void *param)
{
	return RecForEachIMP(tree->root, func, &param);
}

static avl_node_t* CreateNodeIMP(avl_node_t *left_child, avl_node_t *right_child,
														void *data, size_t height)
{
	avl_node_t *new_node = (avl_node_t *)malloc(sizeof(avl_node_t));
	if(NULL == new_node)
	{
		return NULL;
	}
	new_node->children[0] = left_child;
	new_node->children[1] = right_child;
	new_node->data = data;
	new_node->height = height;

	return new_node;
}

static direction_e GetDirectionIMP(int cmp_res)
{
	if ((-1) == cmp_res)
	{
		return LEFT;
	}
	else if ((1) == cmp_res)
	{
		return RIGHT;
	}
	
	return EQUAL;
}
