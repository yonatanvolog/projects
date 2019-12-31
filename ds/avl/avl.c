/**********************************************************************
 *	This code is representing my functions of AVL RC2		          *
 *	Lab: OL-79                                		                  *
 *	Author: Yonatan Vologdin 										  *
 *	Reviewer: Tamir Dayan	 	  							     	  *
 *	Date: 26.12.19													  *
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

typedef enum situation_e
{
	LEFT_LEFT,
	LEFT_RIGHT,
	RIGHT_LEFT,
	RIGHT_RIGHT
}situation_e;

typedef struct avl_node avl_node_t;
struct avl_node
{
	avl_node_t *children[2];
	void *data;
	size_t height; 
};

situation_e situation_LUT[2][2] = {{LEFT_LEFT,  LEFT_RIGHT},
								   {RIGHT_LEFT, RIGHT_RIGHT}};

struct avl
{
	comparison_func_t comparison_func;
	avl_node_t *root;
};

static int IsNodeHasLeftChildIMP(avl_node_t *node);
static int IsNodeHasRightChildIMP(avl_node_t *node);
static int IsNodeLeafIMP(avl_node_t *node);
static int IsNodeHasOnlyLeftChildIMP(avl_node_t *node);
static int IsNodeHasOnlyRightChildIMP(avl_node_t *node);
static int IsNodeHasOnlyOneChildIMP(avl_node_t *node);
static avl_node_t *GetOnlyChildIMP(avl_node_t *node);
static avl_node_t *GetLowestOnLeftIMP(avl_node_t *node);
static avl_node_t *GetNextIMP(avl_node_t *node);
static void SwapNodeDataIMP(avl_node_t *node1, avl_node_t *node2);
static avl_node_t *RotateIMP(direction_e direction, avl_node_t *root);
static avl_node_t *ActivateSituationIMP(avl_node_t *node, situation_e situation);
static int GetBalanceFactorIMP(avl_node_t *node);
static int IsBalancedIMP(int balance_factor);
static situation_e GetSituationIMP(avl_node_t *node);
static avl_node_t *RebalanceIMP(avl_node_t *node);
static int RecForEachIMP(avl_node_t *node, action_func_t func, void *param);
static void RecSizeIMP(avl_node_t *node, size_t *counter);
static void UpdateHeightIMP(avl_node_t *node);
static direction_e GetDirectionIMP(int cmp_res);
void RecDestroyIMP(avl_node_t *node);
static avl_node_t *RemoveSingleNodeIMP (avl_node_t *node);
static avl_node_t *RecFindIMP(avl_node_t *current_root, const void *data, 
												comparison_func_t CmpFunc);
static avl_node_t *RecInsertIMP(avl_node_t *node, void *data, 
									comparison_func_t CmpFunc);
static avl_node_t* CreateNodeIMP(avl_node_t *left_child, avl_node_t *right_child,
													  void *data, size_t height);
static avl_node_t *RecRemoveIMP(avl_node_t *node, const void *data,
										 comparison_func_t CmpFunc);

avl_t *AVLCreate(comparison_func_t func)
{
	avl_t *new_avl = NULL;

	assert(func);

	new_avl = (avl_t *)malloc(sizeof(avl_t));
	if(NULL == new_avl)
	{
		return NULL;
	}
	
	new_avl->comparison_func = func;
	new_avl->root = NULL;

	return new_avl;
}

void AVLDestroy(avl_t *tree)
{
	if (NULL != tree)
	{	
		RecDestroyIMP(tree->root);
		free(tree);
	}
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

int AVLInsert(avl_t *tree, void *data)
{
	avl_node_t *node = NULL;

	assert(tree);
	assert(data);

	node = tree->root;
	node = RecInsertIMP(node, data, tree->comparison_func);
	if (NULL == node)
	{
		return 1;	
	}
	tree->root = node;

	return 0;
}

static avl_node_t *RecInsertIMP(avl_node_t *node, void *data, 
									comparison_func_t CmpFunc)
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

	node = RebalanceIMP(node);
	UpdateHeightIMP(node);

	return node;
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

static avl_node_t *RebalanceIMP(avl_node_t *node)
{
	situation_e situation = 0;

	if (0 == IsBalancedIMP(GetBalanceFactorIMP(node)))
	{
		situation = GetSituationIMP(node);
		node = ActivateSituationIMP(node, situation);
	}

	return node;
}

static int GetBalanceFactorIMP(avl_node_t *node)
{
	int left_child_height = -1;
	int right_child_height = -1;
	
	if (NULL == node)
	{
		return -1;
	}
	
	else if (1 == IsNodeLeafIMP(node))
	{
		return 0;
	}

	else /*node may have 2 children*/
	{
		if (1 == IsNodeHasLeftChildIMP(node))
		{
			left_child_height = node->children[LEFT]->height;
		}
		if (1 == IsNodeHasRightChildIMP(node))
		{
			right_child_height = node->children[RIGHT]->height;
		}
	}
	
	return ((left_child_height) - (right_child_height));
}

static int IsBalancedIMP(int balance_factor)
{
	return ((-1 <= balance_factor) && (balance_factor <= 1));
}


static situation_e GetSituationIMP(avl_node_t *node)
{
	int balance_factor = 0;
	direction_e step[2] = {0};
	size_t iteration = 0;

	for (iteration = 0; iteration < 2; ++iteration)
	{
		balance_factor = GetBalanceFactorIMP(node);

		if (balance_factor > 0)			/*left heavy*/
		{
			node = node->children[LEFT];
		}
		else
		{
			node = node->children[RIGHT];
			step[iteration] += 1;
		}
	}

	return situation_LUT[step[0]][step[1]];
}

static avl_node_t *ActivateSituationIMP(avl_node_t *node, situation_e situation)
{		
	switch (situation)
	{
		case LEFT_LEFT:
			node = RotateIMP(RIGHT, node);
			break;

		case LEFT_RIGHT:
			node->children[LEFT] = RotateIMP(LEFT, node->children[LEFT]);
			node = RotateIMP(RIGHT, node);
			break;

		case RIGHT_LEFT:
			node->children[RIGHT] = RotateIMP(RIGHT, node->children[RIGHT]);
			node = RotateIMP(LEFT, node);
			break;

		case RIGHT_RIGHT:
			node = RotateIMP(LEFT, node);
			break;
	}
	return node;
}

static avl_node_t *RotateIMP(direction_e direction, avl_node_t *root)
{
	avl_node_t *pivot = NULL;
	direction_e	op_direction = LEFT;
	if (LEFT == direction)
	{
		op_direction = RIGHT;
	}

	pivot = root->children[op_direction];
	root->children[op_direction] = pivot->children[direction];
	pivot->children[direction] = root;
	UpdateHeightIMP(root);

	return pivot;
}

static void UpdateHeightIMP(avl_node_t *node)
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

void AVLRemove(avl_t *tree, const void *data)
{
	assert(tree);
	assert(data);
	
	tree->root = RecRemoveIMP(tree->root, data, tree->comparison_func);
}

static avl_node_t *RecRemoveIMP(avl_node_t *node, const void *data,
										 comparison_func_t CmpFunc)
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
	node->children[direction] = 
						RecRemoveIMP(node->children[direction], data, CmpFunc);

	node = RebalanceIMP(node);
	UpdateHeightIMP(node);

	return node;
}

static avl_node_t *RemoveSingleNodeIMP (avl_node_t *node)
{
	avl_node_t *node_child = NULL;
	avl_node_t *node_to_replace = NULL;

	if (1 == IsNodeLeafIMP(node))
	{
		free(node);		
	
		return NULL;
	}
	
	else if (1 == IsNodeHasOnlyOneChildIMP(node))
	{
		node_child = GetOnlyChildIMP(node);
		free(node);
	
		return node_child;
	}
	
	else /*node has 2 children*/
	{
		node_to_replace = GetNextIMP(node);
		SwapNodeDataIMP(node_to_replace, node);
		node->children[RIGHT] = RemoveSingleNodeIMP(node_to_replace);

		UpdateHeightIMP(node);
	}
	
	return node;
}

size_t AVLGetHeight(const avl_t *tree)
{
	assert(tree);

	return (tree->root->height);
}

void *AVLFind(const avl_t *tree, const void *data)
{
	avl_node_t *return_data = NULL;

	assert(tree);
	assert(data);

	return_data = RecFindIMP(tree->root, data, tree->comparison_func);
	if (NULL == return_data)
	{
		return NULL;
	}
	return (return_data->data);
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

int AVLIsEmpty(const avl_t *tree)
{
	assert(tree);
	
	return (NULL == tree->root);
}

size_t AVLSize(const avl_t *tree)
{
	size_t counter = 0;

	assert(tree);

	RecSizeIMP(tree->root, &counter);

	return counter;
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

int AVLForEach(avl_t *tree, action_func_t func, void *param)
{
	assert(tree);
	assert(func);

	return RecForEachIMP(tree->root, func, param);
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
	return (!IsNodeHasRightChildIMP(node) && !IsNodeHasLeftChildIMP(node));
}

static int IsNodeHasOnlyLeftChildIMP(avl_node_t *node)
{
	return (IsNodeHasLeftChildIMP(node) && !IsNodeHasRightChildIMP(node));
}

static int IsNodeHasOnlyRightChildIMP(avl_node_t *node)
{
	return (!IsNodeHasLeftChildIMP(node) && IsNodeHasRightChildIMP(node));
}

static int IsNodeHasOnlyOneChildIMP(avl_node_t *node)
{
	return (IsNodeHasOnlyLeftChildIMP(node) || IsNodeHasOnlyRightChildIMP(node));
}

static avl_node_t *GetOnlyChildIMP(avl_node_t *node)
{
	if (1 == IsNodeHasLeftChildIMP(node))
	{
		return (node->children[LEFT]);
	}

	return (node->children[RIGHT]);
}

static avl_node_t *GetLowestOnLeftIMP(avl_node_t *node)
{
	if (NULL == node->children[LEFT])
	{
		return node;
	}
	return GetLowestOnLeftIMP(node->children[LEFT]);
}

static avl_node_t *GetNextIMP(avl_node_t *node)
{
	return GetLowestOnLeftIMP(node->children[RIGHT]);
}

static void SwapNodeDataIMP(avl_node_t *node1, avl_node_t *node2)
{
	void *tmp = node1->data;

	node1->data = node2->data;
	node2->data = tmp;
}
