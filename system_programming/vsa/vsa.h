/****************************************************************************
*                                                                           *
*                               OL79 - 01/12/19                             *
*                                                                           *	
*                                    VSA                                    *
*                                                                           *
*****************************************************************************/

#ifndef ILRD_VSA_H
#define ILRD_VSA_H
#include <sys/types.h> /* ssize_t */

/*

Paste this in your C file:

typedef struct block_header
{
	ssize_t block_size;
	#ifdef DEBUG
        size_t magic_number;
    #endif
}vsa_block_header_t;

*/

typedef struct block_header vsa_t;

/*	Initialize an array pointed by memory_pool, in size of pool_size.
*	Function receives:
*	memory_pool - pointer to start of memory to be allocated.
*	pool_size - size of memory to be allocated in Bytes.
*	Return Value: a pointer to the new VSA.
*	memory_pool must be a valid allocated memory matching pool_size.
*/
vsa_t *VSAInit(void *memory_pool, size_t pool_size);

/*	The function recieves pointer to VSA.
*	Function allocates one block of block_size bytes via first fit.
*	All free blocks up untill first fit are defragged.
*	Return Value: address of allocated block, 
*	or NULL if there's not enough memory left.
*	VSA must be a valid VSA type that's not NULL.
*/
void *VSAAlloc(vsa_t *vsa, size_t block_size);

/*  This function frees one block of memory pointed by *allocated_address. 
*	allocated_address should have been allocated by VSAInit.
*/
void VSAFree(void *allocated_address);

/*  This function returns size of the biggest contiguous free memory block.
*	The function recieves pointer to VSA.
*	VSA must be a valid VSA type that's not NULL.
*/   
size_t VSALargestChunk(vsa_t *vsa);

#endif
