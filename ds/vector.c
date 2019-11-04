/**********************************************************************
 *	This code is representing my functions of DS_Vector RC2	          *
 *	Lab: OL-79                                		                  *
 *	Author: Yonatan Vologdin 										  *
 *	Reviewer: Erez Barr    				          				      *
 *	Date: 04.11.19													  *
 **********************************************************************/

#include <stdio.h>	/*printf, size_t*/
#include <stdlib.h>	/*realloc*/
#include <string.h>	/*memcpy*/
#include <assert.h>	/*assert*/
#include "vector.h"	/*vector structure*/

#define QUARTER (0.25)
#define MEMORY_INCREASE_FACTOR 2
#define MEMORY_DECREASE_FACTOR 0.5

struct d_vector_t
{
	size_t capacity;
	size_t element_size;
	size_t size;
	void *head;
};

d_vector_t *VectorCreate(size_t capacity, size_t element_size)
{
	d_vector_t *v_ptr = (d_vector_t*)malloc(sizeof(d_vector_t));

	assert(0 < capacity);	
	assert(0 < element_size);
	
	if (NULL == v_ptr)
	{
		return NULL;
	}
	v_ptr->capacity = capacity;
	v_ptr->element_size = element_size;
	v_ptr->size = 0;
	v_ptr->head = (void *)malloc(capacity * element_size);
	
	if (NULL == v_ptr->head)
	{
		free(v_ptr);
		
		return NULL;
	}

	return v_ptr;
}

void VectorDestroy(d_vector_t *vector)
{
	assert(NULL != vector);

	free(vector->head);
	free(vector);
}

int VectorPushBack(d_vector_t *vector, const void *n)
{
	assert(NULL != vector);
	assert(NULL != n);
	
	if (vector->size == vector->capacity)
	{
		if (VectorReserve(vector, vector->capacity * MEMORY_INCREASE_FACTOR))
		{
			return 1;
		}
	}
	memcpy((char *)vector->head + (vector->size * vector->element_size), n, vector->element_size);
	vector->size += 1;
	
	return 0;
}

int VectorReserve(d_vector_t *vector, size_t new_capacity)
{
	void *temp_head = NULL;

	assert(NULL != vector);

	if (new_capacity < vector->size)
	{
		new_capacity = vector->size;
	}
	temp_head = (void *)realloc(vector->head, new_capacity * (vector->element_size));
	
	if (NULL == temp_head)
	{	
		return 1;
	}
	vector->head = temp_head;
	vector->capacity = new_capacity;

	return 0;
}

void *VectorGetItemAddress(const d_vector_t *vector, size_t index)
{
	assert(NULL != vector);

	return ((char *)(vector->head) + index * vector->element_size);
}

void VectorPopBack(d_vector_t *vector)
{
	assert(NULL != vector);
	
	if (vector->size <= vector->capacity * QUARTER)
	{
		vector->capacity *= MEMORY_DECREASE_FACTOR;
		VectorReserve(vector, vector->capacity);
	}
	vector->size -= 1;
}

size_t VectorCapacity(const d_vector_t *vector)
{	
	assert(NULL != vector);

	return vector->capacity;
}

size_t VectorSize(const d_vector_t *vector)
{
	assert(NULL != vector);

	return vector->size;
}
