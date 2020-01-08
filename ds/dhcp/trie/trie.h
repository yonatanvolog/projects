/****************************************************************************
*                                                                           *
*                               OL79 - 02/01/19                             *
*                                                                           *
*                                 Trie										*
*                                                                           *
*****************************************************************************/

#ifndef ILRD_TRIE_H
#define ILRD_TRIE_H

#include <stddef.h> /* size_t */


typedef struct trie trie_t; 
typedef struct node node_t; 

/* * * * * * MOVE TO SOURCE FILE !! * * * * 
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
* * * * * * * * * * * * * * * * * * * * */
typedef enum alloc_status
{
	T_SUCCESS_ALLOCATED_REQUESTED = 0,
	T_REQUESTED_IP_OCCUPIED = 2,
	T_MALLOC_FAIL = 4
}trie_alloc_status_t;

typedef enum free_status
{
	T_SUCCESS = 0,
	T_DOUBLE_FREE = 1,
	T_IP_NOT_FOUND = 2
}trie_free_status_t;

/* Function creates a trie.
 * Receives a level.
 * Return Values:
 *               - returns pointer to the trie.
 *               - if the function fails, it returns NULL. */
trie_t *TrieCreate(size_t level);

/* The function destroys the trie and frees all memory allocations. */
void TrieDestroy(trie_t *trie);


/*	The function recieves requested_ip and result (binary ip).
*	Function allocates ip address.
*	The allocated address will be saved in the result address  
*	If requested address equals to 0, the result address will not be affected by it.
*	If allocation fails result address will be NULL
*	Inside functions are the following actions:
*	Checks if address is valid
*	Checks if address is already allocated and either allocates it or chooses
*	another one.
*	If trie is full returns TRIE_FULL
*/
trie_alloc_status_t TrieInsert(trie_t *trie, unsigned int requested_ip);

/* reset all flags of nodes in the path of the ip edge */
trie_free_status_t TrieDeallocate(trie_t *trie, unsigned int ip);

/* */
size_t TrieCountAlloc(trie_t *trie);

/* */
size_t TrieCountFree(trie_t *trie);

/* */ 
int TrieIsFull(trie_t *trie);

#endif
