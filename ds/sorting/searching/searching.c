/**********************************************************************
 *	This code is representing my searching functions RC2              *
 *	Lab: OL-79                                		                  *
 *	Author: Yonatan Vologdin 										  *
 *	Reviewer: Eyal Rabinovitz           	 	  			     	  *
 *	Date: 07.01.20 													  *
 **********************************************************************/
#include <math.h>   /*sqrt*/

#include "searching.h"

int *BSearch(const int *arr, size_t size, int val)
{
    int *middle = (int *)arr + size / 2;
    int *last_element = (int *)arr + size - 1;
    
    if ((1 == size) && (val != *middle))
    {   
        return NULL;
    }
    
    if (val < *middle)
    {
        return BSearch((int *)arr, size / 2, val);
    }
    
    else if (*middle < val)
    {
        if (last_element == middle)
        {
            return NULL;
        }
    
        return BSearch(middle + 1, size - (size / 2) - 1, val);
    }
    
    return middle;
}

int *JSearch(const int *arr, size_t size, int val)
{
    size_t jump_size = sqrt(size);
    size_t current_index = 0;
    size_t last_index = size - 1;
    
    while ((current_index + jump_size < last_index) && 
           (*(arr + current_index + jump_size) < val))
    {
        current_index += jump_size;
    }
    
    while (current_index <= last_index)
    {
        if (val == *(arr + current_index))
        {
            return (int *)(arr + current_index);
        }
        ++current_index;
    }
 
    return NULL;
}
