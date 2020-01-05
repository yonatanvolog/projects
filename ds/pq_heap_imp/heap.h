/****************************************************************************
*                                                                           *
*                               OL79 - 26/12/19                             *
*                                                                           *
*                                 Heap                                      *
*                                                                           *
*****************************************************************************/

#ifndef ILRD_HEAP_H
#define ILRD_HEAP_H

#include <stddef.h> /* size_t */
#include "vector.h"
#include "heapify.h"

typedef struct heap heap_t;
typedef int (*is_match_t)(const void *new_data, const void *src_data);

/* * * * * * MOVE TO SOURCE FILE !! * * * * 
struct heap
{
	d_vector_t *vector;
	comparison_t comparison_func;
	void *param;
	
};
* * * * * * * * * * * * * * * * * * * * */


/* Function creates a Heap data structure.
 * Receives a comparison function pointer and match function param pointer.
 * The heap_func can't be NULL.
 * comparison function should return (1) if new_data is with higher priority, 
 * (-1) if the new_data is lower priority and (0) if equal.
 * Return Values:
 *                - returns pointer to the Heap.
 *                - if the function fails, it returns NULL. */
heap_t *HeapCreate(comparison_t comparison_func, void *comparison_param);

/* The function destroys the Heap and frees all memory allocations. */
void HeapDestroy(heap_t *heap);

/* The function inserts new data to the Heap according to the heap function.
 * If the data already exists in the Heap, it will insert duplicate data.
 * The pointer must point to a valid Heap. 
 * Function will insert the new data according to the comparison function.
 * Return Value: returns 0 if success, non-zero value otherwise.  */
int HeapPush(heap_t *heap, void *data);

/* Function removes the data that is the highest priority.
 * After HeapPop call, the access data will be lost!
 * The heap pointer must point to a valid heap. 
 * If data is not found, then function is terminated  
 * Popping an empty heap will result in undefined behavior. */
void HeapPop(heap_t *heap);

/* Function returns the data that is the highest priority.
 * Peeking an empty heap will return NULL.
 * The heap pointer must point to a valid Heap. */
void *HeapPeek(const heap_t *heap);

/* The function receives Heap pointer, a match function & match param.
 * The heap pointer match function must be valid.  
 * Return Values of match function: 1 if match, 0 otherwise.
 * Return Values of HeapRemove: 0 if success, non-zero value otherwise. */ 
void *HeapRemove(heap_t *heap, is_match_t is_match_func, void *param);

/* The function receives Heap pointer.
 * Return Values: 1 if the table contains no elements, 0 otherwise.
 * The pointer must point to a valid Heap. */ 
int HeapIsEmpty(const heap_t *heap);

/* The function receives a pointer to the Heap.
 * The Function returns the size of the Heap.
 * The pointer must point to a valid Heap. */
size_t HeapSize(const heap_t *heap);

#endif
