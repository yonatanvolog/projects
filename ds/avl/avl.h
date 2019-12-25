/****************************************************************************
*                                                                           *
*                               OL79 - 23/12/19                             *
*                                                                           *	
*                                   AVL Tree                                *
*                                                                           *
*****************************************************************************/

#ifndef ILRD_AVL_H
#define ILRD_AVL_H

#include <stddef.h> /* size_t */


typedef struct avl avl_t;
typedef int (*comparison_func_t)(const void *new_data, const void *src_data);
typedef int (*action_func_t)(void *data, void *for_each_param);

typedef int (*is_match_t)(void *data, void *func_param);
typedef int (*should_remove_t)(void *data, void *func_param);


/* Function creates a Binary AVL Search Tree struct.
 * Receives the sort function pointer
 * returns pointer to the Binary AVL Search Tree struct header
 * The comparison_func should return 1 if new_data is larger,
 * -1 if src_data is larger and 0 if they are equal.
 * if the function fails, it returns NULL */
avl_t *AVLCreate(comparison_func_t func);

/* The function destroys all nodes in tree and frees all memory allocations,
 * In Post-order. */
void AVLDestroy(avl_t *tree);

/* The function inserts a new node to the correct sorted place
 * according to the sorting function.
 * The function returns an allocation status.
 * If the same data already exists in the struct, the behavior is undefined
 * The pointer must point to a valid AVL struct 
 * Function will insert the new node to optimized location. 
 * returns 0 if success, non-zero if otherwise. 
 * If Tree is Empty, function is undefined. */
int AVLInsert(avl_t *tree, void *data);

/* Function removes the node that is matching with data sent.
 * After AVLRemove call, the access data will be lost!
 * The tree pointer must point to a valid AVL tree 
 * If data is not found then function is terminated */
void AVLRemove(avl_t *tree, const void *data);

/* Function returns height of tree (distance of furthest leaf from root).
 * The tree pointer must point to a valid AVL tree.
 * If tree is empty than result is undefined. */
size_t AVLGetHeight(const avl_t *tree);

/* The function should look for data in the tree.
 * The function loops over entire tree until found.
 * If finds a match, it will return pointer to data.
 * else - it will return NULL.
 * The pointer must point to a valid AVL struct */
void *AVLFind(const avl_t *tree, const void *data);

/* The function receives AVL Binary Search Tree struct pointer.
 * The function returns 1 if the list is Empty and 0 if not
 * The pointer must point to a valid AVL struct */ 
int AVLIsEmpty(const avl_t *tree);

/* The function receives AVL Binary Search Tree struct pointer.
 * The Function returns the size of the tree
 * The pointer must point to a valid AVL struct 
 * Func will run at Pre-order */ 
size_t AVLSize(const avl_t *tree);

/* The function loops over tree, and for every node it will pass the data,
 * alongside param, to the action_func function. 
 * the action_func function can do whatever you want, 
 * but it should return a non - zero value if it fails.
 * If the AVLForEach function recieves non - zero value from action_func, 
 * it will stop with a fail.
 * the AVLForEach will return 0 if completed its task, 
 * else the non - zero value it received from the action_func function 
 * Func will run at In-order */
int AVLForEach(avl_t *tree, action_func_t func, void *param);

/*
BONUS FUNCS!
void AVLMultiFind(avl_t *tree, dl_list_t *list,
			 is_match_t func, void* func_param);

void AVLMultiRemove(avl_t *tree, should_remove_t func, void *func_param);
*/

#endif
