/****************************************************************************
*                                                                           *
*                               OL79 - 03/11/19                             *
*                                                                           *	
*                                   Vector                                  *
*                                                                           *
*****************************************************************************/
#include <stdio.h>
#ifndef VECTOR_H
#define VECTOR_H

typedef struct d_vector_t d_vector_t;

/* This function recieves starting capacity and element size,
  and creates an array of any suitable type. 
  Notice: it is not possible to create a vector of mixed data types.
  0 elements_size and 0 capacity will have undefined behaviour. 
  Return value: a pointer to d_vector_t data type. 
*/
d_vector_t *VectorCreate(size_t capacity, size_t elements_size);

/* This function destroys a vector which was created by VectorCreate.
   Notice: calling this function with any other vector will result in 
   undefined behavior. */
void VectorDestroy(d_vector_t *vector);

/* This function adds an element to the back of the vector.
   Momory increase strategy:
   if(size == capacity) => 	capacity*=2
   Return value: 0 - operation succeeded, 1 - operation failed. 
*/
int VectorPushBack(d_vector_t *vector, const void *n);

/* This function reallocates capacity to the vector, according to user input
  Note that after Push and Pop - Capacity will be updated according to strategy.
  Note that if new capacity lower than size - current capacity will be size.
*/
int VectorReserve(d_vector_t *vector, size_t new_capacity);

/* This function returns the index-located address in the vector
   Notice: attempting to get index from an empty index location will result in 
   undefined behavior. 
   Be advised that upon Pop/Push/Reserve - All previous address will be 
   undefined. 
*/
void *VectorGetItemAddress(const d_vector_t *vector, size_t index);

/* This function extracts the last element of the vector.
   Notice: attempting to pop from an empty vector will result in 
   undefined behavior. 
   Momory decrease strategy:
   if(size == (0.25*capacity)) => capacity*=0.5
  */
void VectorPopBack(d_vector_t *vector);

/* This function returns the current capacity of the vector 
*/
size_t VectorCapacity(const d_vector_t *vector);

/* This function returns the current size (used capacity) of the vector 
*/
size_t VectorSize(const d_vector_t *vector);

#endif
