/****************************************************************************
*                                                                           *
*                               OL79 - 17/11/19                             *
*                                                                           *	
*                                   Binary Tree                             *
*                                                                           *
*****************************************************************************/

#ifndef ILRD_BST_H
#define ILRD_BST_H

#include <stddef.h> /* size_t */

typedef struct bst_node bst_node_t;
typedef bst_node_t *bst_iter_t;
typedef struct bst bst_t;
typedef int (*comparison_func)(const void *new_data, 
							   const void *src_data, 
							   const void *comp_param);
typedef int (*action_func)(void *data, void *for_each_param);

/*
Paste this in your C file:

struct bst_node
{
	struct bst_node *parent; ------------------bst_iter_t parent
	struct bst_node *left;
	struct bst_node *right;
	void *data;
};

struct bst
{
	comparison_func comparison_func_ptr;
	void *comparison_param;
	struct bst_node stub; ---------------------bst_node_t stub
};

*/

/* Function creates a Binary Search Tree struct.
 * Receives the sort function pointer and parameter 
 * returns pointer to the Binary Search Tree struct header
 * The comparison_func should return 1 if new_data is larger,
 * -1 if src_data is larger and 0 if they are equal.
 * if the function fails, it returns NULL */
bst_t *BSTCreate(comparison_func func, void *param);

/*	The function destroys all nodes in tree and frees all memory allocations */
void BSTDestroy(bst_t *tree);

/* The function inserts a new node to the correct sorted place
 * according to the sorting function.
 * The function returns an iterator to the new node.
 * If the same data already exists in the struct, the behavior is undefined
 * if the function fails, it returns an iterator to the end of the list exclusive
 * The pointer must point to a valid BTS struct */
bst_iter_t BSTInsert(bst_t *tree, void *data);

/* Function removes the node pointed by the iterator
 * After BSTRemove call, the access data will be lost!
 * The pointer must point to a valid node in BTS struct */
void BSTRemove(bst_iter_t iter);

/* The function loops over every node. 
 * The ptr function should look for data in the node that is passed to it. 
 * If ptr finds a match, it should return 1.
 * If ptr found a match, The BSTFind returns a iterator to the matched node.
 * else - it will return an iterator to the end iterator exclusive.
 * The pointer must point to a valid BTS struct */
bst_iter_t BSTFind(bst_t *tree, void *data);

/* The function receives Binary Search Tree struct pointer.
 * The function returns 1 if the list is Empty and zero if not
 * The pointer must point to a valid BTS struct */ 
int BSTIsEmpty(const bst_t *tree);

/* The function receives a Binary Search Tree struct pointer.
 * The Function returns the size of the struct
 * The pointer must point to a valid BTS struct */ 
size_t BSTSize(const bst_t *tree);

/* The function loops over every node, from begin(inclusive) to end (exclusive),
 * and passes it alongside param, to the for_one_func function. 
 * the for_one_func function can do whatever you want, 
 * but it should return a non - zero value if it fails.
 * If the BSTForEach functions recieves non - zero value from for_each_func, 
 * it will stop the foreach with a fail.
 * the BSTForEach will return 0 if completed its task, the non - zero value
 * if received from the for_one_func function */
int BSTForEach(bst_iter_t begin, bst_iter_t end, action_func func, void *param);

/* Function receives iterator. Function returns next iterator
 * If iterator is the end node or NULL this will result in undefined behavior */
bst_iter_t BSTNext(bst_iter_t iter);

/* Function receives iterator. Function returns previous iterator
 * If iterator is the begin node or NULL it will result in undefined behavior */
bst_iter_t BSTPrev(bst_iter_t iter);

/* Function receives list. Function returns iterator to start of list inclusive
 * The pointer must point to a valid BTS struct */ 
bst_iter_t BSTBegin(const bst_t *tree);

/* Function receives list. Function returns iterator to end of list exclusive
 * The pointer must point to a valid BTS struct */ 
bst_iter_t BSTEnd(const bst_t *tree);

/* Function receives iterator. Function returns data located inside.
 * The iterator must point to a valid node in BTS struct */
void *BSTGetData(bst_iter_t iter);

/* Function receives two iterators. Function returns 1 if the are equal 
 * and 0 if not.
 * The iterator must point to a valid node in BTS struct */
int BSTIsSameIterator(bst_iter_t iter1, bst_iter_t iter2);

#endif
