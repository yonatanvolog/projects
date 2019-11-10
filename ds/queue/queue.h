/****************************************************************************
*                                                                           *
*                               OL79 - 6/11/19                              *
*                                                                           *	
*                                   Queue                                   *
*                                                                           *
*****************************************************************************/
#ifndef ILRD_QUEUE_H
#define ILRD_QUEUE_H
#include <stddef.h> 

typedef struct queue queue_t;

/* This function creates a queue of any suitable type and inserts data to first place in the queue. 
   Return value: a pointer to queue_t data type. */
queue_t *QCreate();

/* This function destroys a queue which was created only by QCreate. the function also dealloctes all the memory that was allocated.
   Notice: calling this function with any other queue will result in 
   undefined behavior. */
void QDestroy(queue_t *queue);

/* This function adds an element to the end of the queue.
   Return values: 0 - operation succeeded, 1 - operation failed. 
   Data can't be NULL
   queue must be created by QCreate */
int QEnqueue(queue_t *queue, void *data);

/* This function extracts the first element of the queue.
   Notice: attempting to dequeue from an empty queue will result in 
   undefined behavior (use QIsEmpty before QDequeue) 
   queue must be created by QCreate */
void QDequeue(queue_t *queue);

/* This function returns the value of the first element of the queue.
*  Notice: attempting to peek from an empty queue will result in 
   undefined behavior (use QIsEmpty before QPeek) 
   queue must be created by QCreate */
void *QPeek(const queue_t *queue);

/* This function checks if the queue is empty. 
   Return values: 0 - not empty, 1 - empty. 
   queue must be created by QCreate */
int QIsEmpty(const queue_t *queue);

/* This function returns the size (number of elements) of the queue 
   queue must be created by QCreate */
size_t QSize(const queue_t *queue);

/* This function adds the elements in the src queue after the last element of dst queue (meaning - the first inserted element of src will be conected to the last inserted element of dst)
   Src and dest must be queues created by QCreate.*/ 
void QAppend(queue_t *dest, queue_t *src); 

#endif
