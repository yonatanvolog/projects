/**********************************************************************
 *	This code is representing my functions of Trie RC2                *
 *	Lab: OL-79                                		                  *
 *	Author: Yonatan Vologdin 										  *
 *	Reviewer: Erez Barr              	 	  	    		     	  *
 *	Date: 05.01.20  												  *
 **********************************************************************/
#include <stdlib.h> /*malloc, free*/
#include <assert.h> /*assert*/
#include <math.h>   /*pow*/

#include "trie.h"

#define BYTE_SIZE (8)
#define IP_SIZE (4)
#define BITS_IN_IP ((BYTE_SIZE)*(IP_SIZE))

struct trie 
{
	node_t *node;
	size_t level;
};

struct node 
{
	node_t *children[2];
	int is_subtree_full;
};

typedef enum children_direction_t
{
    LEFT = 0,
    RIGHT = 1
}children_direction_t;

enum 
{
    FALSE = 0,
    TRUE = 1
};

enum 
{
    YES = 1,
    NO = 0
};

static node_t *CreateNodeIMP(void);
static node_t *RecTrieInsertIMP(node_t *node,
                                size_t level,
                                unsigned int requested_ip,
                                trie_alloc_status_t *status);
static void RecTrieDeallocateIMP(node_t *node,
                                 size_t level,
                                 unsigned int ip,
                                 trie_free_status_t *status);
static void RecTrieCountAllocIMP(node_t *node, size_t level ,size_t *counter);
static int SubtreeIsFullIMP(node_t *node);
static int HasTwoChildsIMP(node_t *node);
static void RecTrieDestroyIMP(node_t *node);
static unsigned int GetDirectionAndAdvanceIMP(unsigned int *ip);

trie_t *TrieCreate(size_t level)
{
    trie_t *new_trie = (trie_t *)malloc(sizeof(trie_t));
    if (NULL == new_trie)
    {
        return NULL;
    }

    new_trie->node = CreateNodeIMP();
    if (NULL == new_trie->node)
    {
        free(new_trie);
        
        return NULL;
    }

    new_trie->level = level;
    
    return new_trie;
}

static node_t *CreateNodeIMP(void)
{
    node_t *new_node = (node_t *)malloc(sizeof(node_t));
    if (NULL == new_node)
    {
        return NULL;
    }

    new_node->children[LEFT] = NULL;
    new_node->children[RIGHT] = NULL;
    new_node->is_subtree_full = 0;
    
    return new_node;
}

void TrieDestroy(trie_t *trie)
{
    if (NULL != trie)
	{	
		RecTrieDestroyIMP(trie->node);
		free(trie);
	}
}

/*Post-order*/
static void RecTrieDestroyIMP(node_t *node)
{
	if (NULL == node)
	{
		return;
	}

	RecTrieDestroyIMP(node->children[LEFT]);
	RecTrieDestroyIMP(node->children[RIGHT]);
	node->children[LEFT] = NULL;
	node->children[RIGHT] = NULL;
	free(node);
}

trie_alloc_status_t TrieInsert(trie_t *trie, unsigned int requested_ip)
{   
    trie_alloc_status_t status = T_SUCCESS_ALLOCATED_REQUESTED;
    node_t *node = NULL;

    assert(trie);

    requested_ip <<= (BITS_IN_IP - trie->level);    
    node = trie->node;
    node = RecTrieInsertIMP(node, trie->level, requested_ip, &status);
    
    return status;
}

static node_t *RecTrieInsertIMP(node_t *node,
                                size_t level,
                                unsigned int requested_ip,
                                trie_alloc_status_t *status)
{
    unsigned int direction = GetDirectionAndAdvanceIMP(&requested_ip);

    if (NULL == node)
    {
        node = CreateNodeIMP();
        if (NULL == node)
        {
            *status = T_MALLOC_FAIL;
            
            return NULL;
        }
    }
    
    if (YES == node->is_subtree_full)
    {
        *status = T_REQUESTED_IP_OCCUPIED;
        
        return node;
    }
    
    if (0 == level)
    {
        node->is_subtree_full = YES;
        
        return node;
    }
    
    node->children[direction] = RecTrieInsertIMP(node->children[direction],
                                                 level - 1,
                                                 requested_ip,
												 status);
    
    if (HasTwoChildsIMP(node))
    {
         node->is_subtree_full = SubtreeIsFullIMP(node);
    } 

	return node;
}

static unsigned int GetDirectionAndAdvanceIMP(unsigned int *ip)
{
    unsigned int direction = 0;
	unsigned int msb_mask = 1;
    msb_mask <<= (BITS_IN_IP - 1);

    direction = (*ip & msb_mask) >> (BITS_IN_IP - 1);
	
	*ip <<= 1;
	
	return direction;
}

static int SubtreeIsFullIMP(node_t *node)
{
    return ((YES == node->children[RIGHT]->is_subtree_full) &&
            (YES == node->children[LEFT]->is_subtree_full)); 
}

static int HasTwoChildsIMP(node_t *node)
{
    return ((NULL != node->children[RIGHT]) && (NULL != node->children[LEFT]));
}

trie_free_status_t TrieDeallocate(trie_t *trie, unsigned int ip)
{
    trie_free_status_t status = T_SUCCESS;
    node_t *node = NULL;

    assert(trie);

    ip <<= (BITS_IN_IP - trie->level);    

    node = trie->node;
    RecTrieDeallocateIMP(node, trie->level, ip, &status);
    
    return status;
}

static void RecTrieDeallocateIMP(node_t *node,
                                    size_t level,
                                    unsigned int ip,
                                    trie_free_status_t *status)
{
    unsigned int direction = GetDirectionAndAdvanceIMP(&ip);
    
    if (NULL == node)
    {
        *status = T_IP_NOT_FOUND;
        
        return;
    }
    
    if ((0 == level) && (NO == node->is_subtree_full))
    {
        *status = T_DOUBLE_FREE;
        
        return;
    }
    
    if ((0 == level) && (YES == node->is_subtree_full))
    {
        node->is_subtree_full = NO;
        *status = T_SUCCESS;
        
        return;
    }
    
    
    RecTrieDeallocateIMP(node->children[direction], level - 1, ip, status);
    node->is_subtree_full = NO;
}

size_t TrieCountAlloc(trie_t *trie)
{
    size_t counter = 0;
    
    assert(trie);
    
    RecTrieCountAllocIMP(trie->node, trie->level, &counter);
	
	return counter;
}

/*Pre-order*/ 
static void RecTrieCountAllocIMP(node_t *node, size_t level ,size_t *counter)
{
	if (NULL == node)
	{
		return;
	}

    if ((0 == level) && (YES == node->is_subtree_full))
	{
	    *counter += 1;
	    
		return;
	}

	RecTrieCountAllocIMP(node->children[LEFT], level -1, counter);
	RecTrieCountAllocIMP(node->children[RIGHT], level -1, counter);
}

size_t TrieCountFree(trie_t *trie)
{
    size_t free_in_potential = 0;
    size_t actual_free = 0;
    
    assert(trie);
    
    free_in_potential = pow(2, trie->level);
    actual_free = free_in_potential - TrieCountAlloc(trie);
    
    return actual_free;   
}

int TrieIsFull(trie_t *trie)
{
    assert(trie);
    
    return (YES == trie->node->is_subtree_full);
}
