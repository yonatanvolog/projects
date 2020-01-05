/****************************************************************************
*                                                                           *
*                               OL79 - 26/12/19                             *
*                                                                           *
*                                 Heapify                                   *
*                                                                           *
*****************************************************************************/

#ifndef ILRD_HEAPIFY_H
#define ILRD_HEAPIFY_H

#include <stddef.h> /* size_t */

typedef int (*comparison_t)(const void *new_data, 
							const void *src_data,
							void *compare_param);

/* The function sift up the element found in index_of_heapify location
 * in the array.
 */
void HeapifyUp(void *arr, 
				size_t arr_size, 
				size_t elem_size, 
			    size_t index_of_heapify, 
			    comparison_t func, 
			    void *compare_param);
			    
/* The function sift down the element found on top location to index_of_heapify
in the array */
void HeapifyDown(void *arr, 
				  size_t arr_size, 
				  size_t elem_size, 
			      size_t index_of_heapify, 
			      comparison_t func, 
			      void *compare_param);

#endif
