/**********************************************************************
 *	This code is representing my functions of Heapify RC2	          *
 *	Lab: OL-79                                		                  *
 *	Author: Yonatan Vologdin 										  *
 *	Reviewer: Chen Rubin    				          			      *
 *	Date: 03.01.20													  *
 **********************************************************************/

#include <assert.h>	/*assert*/
#include <alloca.h>	/*alloca*/
#include <string.h>	/*memcpy*/

#include "heapify.h"
#include "../../yoni_utils.h" /*UNUSE*/

#define PARENT(i)       (((i)  - 1)/2)
#define LEFT_CHILD(i)   ((2 * (i)) + 1)
#define RIGHT_CHILD(i)  ((2 * (i)) + 2)
#define VECTOR_CAPACITY (10)
#define HIGHER_PRIORITY (1)
#define ROOT_INDEX 		(0)
#define SET_PTR_TO(arr, set_to, elem_size)\
	(((char *)arr) + ((set_to) * (elem_size)))

typedef enum direction_t
{
	LEFT = 0,
	RIGHT = 1,
	NO_CHILD = 2
}direction_t;

typedef enum num_of_children_t
{
	ZERO_CHILD = 0,
	ONE_CHILD = 1,
	TWO_CHILD = 2
}num_of_children_t;

static void SwapIMP(void *new_data_ptr, void *parent_ptr, size_t elem_size);
static int IsHasChildrenIMP(size_t index, size_t arr_size);
static num_of_children_t GetNumOfChildrenIMP(size_t left_child_index,
								             size_t right_child_index,
								             size_t arr_size);
static size_t CmpAndSwapTwoElementsIMP(void *data1, void *data2,
									   size_t index, size_t elem_size,
									   comparison_t func, void *compare_param);


void HeapifyUp(void *arr, 
			   size_t arr_size, 
			   size_t elem_size, 
			   size_t index_of_heapify, 
			   comparison_t func, 
			   void *compare_param)
{
    size_t heapify_parent = PARENT(index_of_heapify);       
    void *new_data_ptr = NULL;
    void *parent_ptr   = NULL;
    
  	assert(arr);
	assert(func);

    new_data_ptr = SET_PTR_TO(arr, index_of_heapify, elem_size);
    parent_ptr   = SET_PTR_TO(arr ,heapify_parent , elem_size);

    if ((ROOT_INDEX == index_of_heapify) ||
		(0 == func(new_data_ptr, parent_ptr, compare_param)))
    {
		return;
    }

	SwapIMP(new_data_ptr, parent_ptr, elem_size);
	HeapifyUp(arr, arr_size, 
  			  elem_size,
			  PARENT(index_of_heapify),
           	  func, compare_param);
}

void HeapifyDown(void *arr, 
			     size_t arr_size, 
			     size_t elem_size, 
			     size_t index_of_heapify, 
			     comparison_t func, 
			     void *compare_param)
{
    void *parent = NULL;
    void *left_child = NULL;
    void *right_child = NULL;
    size_t left_child_index = LEFT_CHILD(index_of_heapify);
    size_t right_child_index = RIGHT_CHILD(index_of_heapify);
    size_t highest_priority_child_index = 0;
    num_of_children_t num_of_children = GetNumOfChildrenIMP(left_child_index,
                                               			    right_child_index,
                                                            arr_size);
    assert(arr);
    assert(func);

    parent      = SET_PTR_TO(arr, index_of_heapify, elem_size);
    left_child  = SET_PTR_TO(arr, left_child_index, elem_size);
    right_child = SET_PTR_TO(arr, right_child_index, elem_size);
	
	switch (num_of_children)
	{
		case ZERO_CHILD:
			return;
			break;

		case ONE_CHILD:
			highest_priority_child_index =
							   CmpAndSwapTwoElementsIMP(parent, left_child, 
                                                        left_child_index, 
                                                        elem_size,
                                                        func,compare_param);
			break;

		case TWO_CHILD:
			if (HIGHER_PRIORITY == func(left_child, right_child, compare_param))
    		{
				highest_priority_child_index =  
								CmpAndSwapTwoElementsIMP(parent,left_child, 
														 left_child_index, 
														 elem_size, 
													     func, compare_param);        
    		}
			else
			{
		
				 highest_priority_child_index =  
								CmpAndSwapTwoElementsIMP(parent,right_child, 
		    											 right_child_index, 
														 elem_size, 
												         func, compare_param);  
			}
			break;
	}

    if (ROOT_INDEX < highest_priority_child_index)
    {
        HeapifyDown(arr, arr_size,
  				    elem_size,
				    highest_priority_child_index, 
                    func, compare_param);
    }
}


static void SwapIMP(void *data1, void *data2, size_t elem_size)
{
    void *tmp = alloca(elem_size);
    memcpy(tmp, data1, elem_size);
    memcpy(data1, data2, elem_size);
    memcpy(data2, tmp, elem_size);
}

static int IsHasChildrenIMP(size_t index, size_t arr_size)
{
    return (index < arr_size);
}

static num_of_children_t GetNumOfChildrenIMP(size_t left_child_index,
								             size_t right_child_index,
								             size_t arr_size)
{
    if (!IsHasChildrenIMP(left_child_index, arr_size))
    {
        return ZERO_CHILD;
    }

    else if ((!IsHasChildrenIMP(right_child_index, arr_size)))
    {
        return ONE_CHILD;
    }

    return TWO_CHILD;
}

static size_t CmpAndSwapTwoElementsIMP(void *data1, void *data2,
									   size_t index, size_t elem_size,
									   comparison_t func, void *compare_param)
{
    if (1 > func(data1, data2, compare_param))
    {
        SwapIMP(data1, data2, elem_size);

        return index;
    }

    return ROOT_INDEX;
}
