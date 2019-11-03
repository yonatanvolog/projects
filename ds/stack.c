/**********************************************************************
 *	This code is representing my functions of DS_Stack RC2            *
 *	Lab: OL-79                                		                  *
 *	Author: Yonatan Vologdin 										  *
 *	Reviewer: Sharon Rottner    								      *
 *	Date: 03.11.19													  *
 **********************************************************************/

#include <stdio.h>	/*printf, size_t*/
#include <stdlib.h>	/*malloc*/
#include <assert.h>	/*assert*/
#include "stack.h"	/*stack structure*/

struct stack_t
{
    size_t elements_size;
    char *head;
    void *tail;
    void *current;
};

stack_t *StackCreate(size_t num_of_elements, size_t elements_size)
{
	stack_t* s_ptr = (stack_t*)malloc(sizeof(stack_t));
	if(NULL == s_ptr)
	{
		return NULL;
	}
	s_ptr->elements_size = elements_size;
	s_ptr->head = (char *)malloc(num_of_elements * elements_size);
	if(NULL == s_ptr->head)
	{
		return NULL;
	}
	s_ptr->tail = (void *)(s_ptr->head + (num_of_elements * elements_size));
	s_ptr->current = (void *)s_ptr->head;
	
	return s_ptr;
}

void StackDestroy(stack_t *stack)
{
	assert(NULL != stack);

	free(stack->head);
	free(stack);
}

int StackPush(stack_t *stack, const void *n)
{
	size_t i = 0;

	assert((NULL != stack) && (NULL != n));

	if (0 == (char *)stack->tail - (char *)stack->current)
	{
		return 1;
	}

	for(i = 0; i < stack->elements_size; ++i)
	{
		*((char *)stack->current) = *((char *)n);
		stack->current = (char *)stack->current + 1;
		n = (char *)n + 1;
	}

	return 0;
}

void StackPop(stack_t *stack)
{
	assert(NULL != stack);

	stack->current = (char *)stack->current - stack->elements_size;
}

void *StackPeek(const stack_t *stack)
{
	assert(NULL != stack);
	
	return ((char *)stack->current - stack->elements_size);
}

int StackIsEmpty(const stack_t *stack)
{
	assert(NULL != stack);
	
	if(stack->current == stack->head)
	{
		return 1;
	}
	
	return 0;	
}

size_t StackSize(const stack_t *stack)
{
	assert(NULL != stack);
	
	return (((char *)stack->current - stack->head) / (stack->elements_size));
}
