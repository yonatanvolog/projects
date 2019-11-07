/**********************************************************************
 *	This code is representing my functions of DS_SList RC3           *
 *	Lab: OL-79                                		                  *
 *	Author: Yonatan Vologdin 										  *
 *	Reviewer: Chen Rubin	    								      *
 *	Date: 07.11.19													  *
 **********************************************************************/
 
#include <stdio.h>	/*printf*/
#include <stdlib.h>	/*malloc*/
#include <assert.h>	/*assert*/

#include "slist.h"	/*singly linked list structure*/

sl_node_t *SListCreateNode(void *data, sl_node_t *next)
{
	sl_node_t *new_node = NULL;

	/*no assert bacause head is initially created with NULLs*/

	new_node = (sl_node_t *)malloc(sizeof(sl_node_t));
	if (NULL == new_node)
	{
		return NULL;
	}
	new_node->data = data;
	new_node->next = next;
	
	return new_node;
}

sl_node_t *SListInsertAfter(sl_node_t *new_node, sl_node_t *pos)
{
	void *pos_next_copy = NULL;
	
	assert(NULL != new_node);
	assert(NULL != pos);

	pos_next_copy = pos->next;
	pos->next = new_node;
	new_node->next = pos_next_copy;
	
	return new_node;
}

sl_node_t *SListInsert(sl_node_t *new_node, sl_node_t *pos)
{
	void *pos_data_copy = NULL;
	sl_node_t* tmp_ptr = pos;
	
	assert(NULL != new_node);
	assert(NULL != pos);

	pos_data_copy = pos->data;
	pos->data = new_node->data;
	new_node->data = pos_data_copy;
	
	new_node->next = pos->next;
	pos->next = new_node;
	
	pos = new_node;
	new_node = tmp_ptr;
	
	return new_node;
}
void SListFreeAll(sl_node_t *node)
{
	sl_node_t *prev_node_ptr = NULL;	
		
	assert(NULL != node);

	while (NULL != node->next)
	{
		prev_node_ptr = node;
		node = node->next; 
		free(prev_node_ptr);
	}
	free(node);
}

size_t SListCount(const sl_node_t *head)
{
	size_t count = 1;

	assert(NULL != head);
	
	while (NULL != head->next)
	{
		head = head->next;
		++count;
	}
	
	return count;
}

sl_node_t *SListRemove(sl_node_t *node)
{
	assert(NULL != node);
	
	if (NULL == node->next)
	{
		return NULL;
	}
	
	node->data = (node->next)->data;
	node->next = (node->next)->next;

	return node;
}

sl_node_t *SListRemoveAfter(sl_node_t *node)
{
	sl_node_t *sl_node_to_remove = NULL;
	
	assert(NULL != node);
	
	if (NULL != node->next)
	{
		sl_node_to_remove = node->next;
		node->next = sl_node_to_remove->next;
		sl_node_to_remove->next = NULL;
	}
	
	return node;
}

int SListHasLoop(sl_node_t *head)
{
	sl_node_t *slow_runner = head;
	sl_node_t *fast_runner = NULL;
	
	assert(NULL != head);
	
	fast_runner = head->next;
	
	while ((slow_runner != fast_runner) && (NULL != fast_runner))
	{
		slow_runner = slow_runner->next;
		
		if (NULL == fast_runner->next)
		{	
			return 0;
		}
		fast_runner = (fast_runner->next)->next;
	}
	
	return 1;
}

sl_node_t *SListFindIntersection(sl_node_t *node1, sl_node_t *node2)
{
	sl_node_t *node1_r = node1;
	sl_node_t *node2_r = node2;
	size_t node1_till_null = 0;
	size_t node2_till_null = 0;
	size_t i = 0;
	
	assert(NULL != node1);
	assert(NULL != node2);
	
	node1_till_null = SListCount(node1);
	node2_till_null = SListCount(node2);
	
	if (node1_till_null > node2_till_null)
	{	
		for(i = 0; i < node1_till_null - node2_till_null; ++i)
		{
			node1 = node1->next;
		}
	}
	
	else if (node1_till_null < node2_till_null)
	{	
		for(i = 0; i < node2_till_null - node1_till_null; ++i)
		{
			node2 = node2->next;
		}
	}
	
	if (node1 == node2)
	{
		return NULL;
	}
	
	while (1)
	{
		while (NULL != node1_r)
		{
			if (node1_r == node2_r)
			{
				return node1_r;
			}
			node1_r = node1_r->next;
		}
		node1_r = node1;
		node2_r = node2_r->next;

		if (NULL == node2_r)
		{
			return NULL;
		}
	}
}

sl_node_t *SListFlip(sl_node_t *head)
{
	sl_node_t *prev_node = NULL;
	sl_node_t *current_node = head;
	sl_node_t *next_node = NULL;
	
	assert(NULL != head);
	
	next_node = head->next;
	
	while (NULL != next_node)
	{
		current_node->next = prev_node;	
		prev_node = current_node;
		current_node = next_node;
		next_node = next_node->next;
	}
	current_node->next = prev_node;
	
	return current_node;
}

int SListForEach(sl_node_t *node, void *param, for_each_ptr ptr)
{
	assert(NULL != node);
	assert(NULL != param);
	
	while (NULL != node)
	{
		if(1 == ptr(node, param))
		{	
			return 1;
		}
		node = node->next;
	}
	
	return 0;
}

const sl_node_t *SListFind(const sl_node_t *node, void *param, find_ptr ptr)
{
	assert(NULL != node);
	assert(NULL != param);

	while (NULL != node)
	{
		if (ptr(node, param))
		{
			return node;
		}
		node = node->next;
	}
	
	return NULL;
}
