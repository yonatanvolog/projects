/****************************************************************************
*                                                                           *
*                            OL79 - 10/11/19                                *
*                                                                           *	
*                            Circular Buffer                                *
*                                                                           *
*****************************************************************************/
#ifndef ILRD_CBUFF_H
#define ILRD_CBUFF_H

#include <stddef.h>     /* For size_t  */
#include <sys/types.h>  /* For ssize_t */

typedef struct CB cbuff_t;

/* Creates a new circular buffer, allocating enough memory to hold buffer's 
 struct fields + user defined capacity + 1. Returns a pointer to created buffer,
 or NULL if allocation failed */
cbuff_t *CBuffCreate(size_t capacity);

/* Frees memory allocated to a circular buffer created with CBuffCreate */
void CBuffDestroy(cbuff_t *cbuff);

/* Attempts to read nbytes from memory buffer cbuff, and write to supplied
 buffer dest. If dest doesn't have enough capacity for write data, behaviour is 
 undefined. If nbytes cannot be fully read, max allowed number is read.
 Returns the number of bytes read, or -1 if reading attempt fails.*/
ssize_t CBuffRead(cbuff_t *cbuff, void *dest, size_t nbytes);
 
/* Attempts to write nbytes to memory buffer cbuff from supplied buffer src,
 while NOT overwriting any previously unread data. If nbytes cannot be fully
 written, max allowed number is written. Returns the number of bytes written,
 or -1 if write attempt fails.*/
ssize_t CBuffWrite(cbuff_t *cbuff, const void *src, size_t nbytes);
  
/* Returns number of bytes in cbuff available for writing - unread data CANNOT
 be overwritten */
size_t CBuffSpaceLeft(const cbuff_t *cbuff);
 
/* Returns 1 if cbuff is empty, otherwise returns 0. */
int CBuffIsEmpty(const cbuff_t *cbuff);

/* Returns total byte capacity of cbuff */
size_t CBuffCapacity(const cbuff_t *cbuff);

#endif
