/****************************************************************************
*                                                                           *
*                               OL79 - 25/11/19                             *
*                                                                           *	
*                                  FSA                                	    *
*                                                                           *
*****************************************************************************/

#ifndef ILRD_FSA_H
#define ILRD_FSA_H
#include <stddef.h> /* size_t */

typedef struct fsa fsa_t;

/*	Initialize an array pointed by memory_pool, in size of pool_size and each 
*	block size is block_size.	
*	Function receives:
*	memory_pool - pointer to start of memory to be allocated
*	(using FASuggestedSize is recommended)
*	pool_size - size of memory to be allocated in Bytes
*	block_size - size of each block of memory in Bytes
*	Return Value: a pointer to the new FSA.
*	memory_pool must be a valid allocated memory matching pool size
*	Pool size should be bigger than block size.
*/
fsa_t *FSAInit(void *memory_pool, size_t pool_size, size_t block_size);

/* This function suggests a proper size for the user to use in his malloc.
*  It takes under consideration all kinds of restrictions such as padding
*  in order to maximize FSA.	 
*  If the user chooses not to use it, it will result in an undefined behavior.
*  FSA must not be NULL	
*/   
size_t FSASuggestedSize(size_t num_of_blocks, size_t block_size);

/*	Function allocates one block 
*	The function recieves pointer to FSA
*	Return Value: address of allocated block
*	FSA must not be NULL 
*/
void *FSAAlloc(fsa_t *fsa);

/*  This function frees one block of memory pointed by *allocated_address. 
*/
void FSAFree(void *allocated_address);

/*  This function counts how many free blocks of memory are left
*	The function recieves pointer to FSA
*	FSA must not be NULL
*/   
size_t FSACountFree(fsa_t *fsa);

#endif
