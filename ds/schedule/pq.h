/****************************************************************************
*                                                                           *
*                               OL79 - 18/11/19                             *
*                                                                           *	
*                               Priority Queue                              *
*                                                                           *
*****************************************************************************/
#ifndef ILRD_P_QUEUE_H
#define ILRD_P_QUEUE_H
#include <stddef.h>

typedef struct p_queue p_queue_t;

/*	This Function creates a Priority Queue. 

*	The function receives a compare function, supplied by the user,
*	which should return 1 if has a bigger priority, 0 if has equal priority 
*	and -1 if has a smaller priority.
	
*	Return Value: a pointer to the priority queue object. 
				  if the function fails, it returns NULL. 
				  
*	Notice: user must supply a valid priority function */
p_queue_t *PQCreate(void *priority_param, int(*compare_func)(const void *new_data, const void *src_data, void *param));

/*  This function destroys a Priority Queue which was created only by PQCreate. 
*	the function also de-alloctes all the memory that was allocated.
*   Notice:	The priority queue cannot be NULL.
*   calling with any other queue will result in undefined behavior. */
void PQDestroy(p_queue_t *p_queue);

/*	The function inserts a new data to the correct place in a priority queue,
*	which was created only by PQCreate, according to compare function provided 
	from the user.
	
*	Return Value: If The function succeeded it returns 0.
*	if the function fails, it returns 1.

*	Notice: calling with any other queue will result in undefined behavior 
*	Notice: The priority queue cannot be NULL */
int PQEnqueue(p_queue_t *p_queue, void *data);

/* This function extracts the first element of the queue (top priority).

	Return Value: void *data of the deleted element.

   Notice: attempting to dequeue from an empty queue will result in 
   undefined behavior (use QIsEmpty before QDequeue)   
   queue must be created by PQCreate */
void *PQDequeue(p_queue_t *p_queue);

/* This function returns the value of the first element of the queue (top priority).

   Notice: attempting to peek from an empty queue will result in 
   undefined behavior (use QIsEmpty before QPeek) 
   queue must be created by PQCreate */
void *PQPeek(const p_queue_t *p_queue);

/* This function checks if the p_queue is empty. 
   Return values: 0 - not empty, 1 - empty. 
   Notice: attempting to check if empty on an empty p_queue will result in 
   undefined behavior. 
   queue must be created by PQCreate */
int PQIsEmpty(const p_queue_t *p_queue);

/* This function returns the size (number of elements) of the p_queue. 
   queue must be created by PQCreate
   Notice: attempting to check size of an empty p_queue will result in 
   undefined behavior. */
size_t PQSize(const p_queue_t *p_queue);

/*  This function clears all elements of the p_queue, 
	but the queue itself remains.
	
	Notice: attempting to clean an empty p_queue will result in 
    undefined behavior. */
void PQClear(p_queue_t *p_queue);

/*  This function erases a specific element in the p_queue. 

	The function receives is_match function supplied by the user, 
	thet returns 1 if has a match, 0 if not.
	
	Return Value: void *data of the deleted element.
	or NULL if it fails to find by value.
	
	Notice: user must supply a valid match function & param 
	
	Notice: attempting to erase an element from an empty p_queue will result in 
    undefined behavior.*/
void *PQErase(void *data, p_queue_t *p_queue, 
 int(*is_match)(const void *new_data, const void *param));

#endif
