/****************************************************************************
*                                                                           *
*                               OL79 - 31/10/19                             *
*                                                                           *	
*                                   Stack                                   *
*                                                                           *
*****************************************************************************/
#include <stdio.h> /* size_t */

#ifndef STACK_H
#define STACK_H

typedef struct stack_t stack_t;

/* This function recieves number of elements and elements size,
* and creates a stack of any suitable type. 
* Notice: it is not possible to create a stack of mixed data types.
* Return value: a pointer to stack_t data type. */
stack_t *StackCreate(size_t num_of_elements, size_t elements_size);

/* This function destroys a stack which was created only by StackCreate.
   Notice: calling this function with any other stack will result in 
   undefined behavior. */
void StackDestroy(stack_t *stack);

/* This function adds an element to the head of the stack.
   Notice: attempting to push to a full stack will result in undefined behavior. 
*  Return value: 0 - operation succeeded, 1 - operation failed. */
int StackPush(stack_t *stack, const void *n);

/* This function extracts the first element of the stack.
   Notice: attempting to pop from an empty stack will result in 
   undefined behavior. */
void StackPop(stack_t *stack);

/* This function returns the first element of the stack without extracting it
*  Notice: attempting to peek from an empty stack will result in 
   undefined behavior. */
void *StackPeek(const stack_t *stack);

/* This function checks if the stack is empty. 
   Return value: 0 - not empty, 1 - empty. */
int StackIsEmpty(const stack_t *stack);

/* This function returns the size (number of elements) of the stack */
size_t StackSize(const stack_t *stack);

#endif
