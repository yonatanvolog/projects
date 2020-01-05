/**********************************************************************
 *	This code is representing my functions of FSA RC2		          *
 *	Lab: OL-79                                		                  *
 *	Author: Yonatan Vologdin 										  *
 *	Reviewer: Eyal Finkelshtein  		  							  *
 *	Date: 26.11.19													  *
 **********************************************************************/
#include <assert.h>		/*assert*/

#include "fsa.h"		/*fsa*/
#include "../../yoni_utils.h"

#define FSA_SIZE	sizeof(fsa_t)
#define HEADER_SIZE	sizeof(fsa_block_header_t)

static void SwapOffsetIMP(size_t *offset_of_block, size_t *offset_of_struct);

struct fsa
{
	size_t next_free;
	size_t block_size;
};

typedef struct block_header
{
	size_t offset;
}fsa_block_header_t;

/*	|_FSA_||_header_,______writable memory___,__padding__|
 *  0     16        24                                   fsa+header+mem+padding
		   ^											 (i.e. next_free)
 *         |
 *	next_free/ offset (number of butes from beginning of FSA)
 */
fsa_t *FSAInit(void *memory_pool, size_t pool_size, size_t block_size)
{
	char *memory_pool_runner	= memory_pool;
	fsa_t *fsa 					= memory_pool;
	fsa_block_header_t *header	= NULL;
	size_t total_block_size	= 0;
	size_t num_of_blocks	= 0;
	size_t i				= 0;
	size_t block_padding	= WORD_SIZE - (block_size % WORD_SIZE);
	if (0 == block_size % WORD_SIZE)
	{
		block_padding = 0;
	}

	total_block_size = HEADER_SIZE + block_size + block_padding;

	num_of_blocks 	= (pool_size - FSA_SIZE) / total_block_size;

	assert(memory_pool);
	
	fsa->next_free	= FSA_SIZE;	/*offset in bytes*/
	fsa->block_size	= total_block_size;

	/*moving init_runner to beginning of first block*/
	memory_pool_runner += FSA_SIZE;
	
	for(i = 1; i < num_of_blocks; ++i)
	{
		header				= (fsa_block_header_t *)memory_pool_runner;
		header->offset		= FSA_SIZE + i * total_block_size;
		memory_pool_runner += total_block_size;
	}

	header = (fsa_block_header_t *)memory_pool_runner;
	header->offset = 0;	

	return fsa;
}

size_t FSASuggestedSize(size_t num_of_blocks, size_t block_size)
{
	size_t block_padding	= WORD_SIZE - (block_size % WORD_SIZE);
	size_t total_block_size = HEADER_SIZE + block_size + block_padding;

	return (FSA_SIZE + num_of_blocks * total_block_size);
}

void *FSAAlloc(fsa_t *fsa)
{
    void *alloc_for_user 		= NULL;
    fsa_block_header_t *header	= NULL;

	assert(fsa);

	header = (fsa_block_header_t*)((char *)fsa + fsa->next_free);
    alloc_for_user = (char *)header + HEADER_SIZE;
    SwapOffsetIMP(&header->offset, &fsa->next_free);

    return alloc_for_user;
}

void FSAFree(void *allocated_address)
{
	fsa_block_header_t *header	= NULL;
	fsa_t *fsa					= NULL;

	char *header_runner	= allocated_address;
	char *fsa_runner	= NULL;

	if (NULL != allocated_address)
	{
		header_runner  -= HEADER_SIZE;
		fsa_runner		= header_runner -
						((fsa_block_header_t *)header_runner)->offset;

		header	= (fsa_block_header_t *)header_runner;
		fsa		= (fsa_t *)fsa_runner;
		SwapOffsetIMP(&header->offset, &fsa->next_free);
	}
}

size_t FSACountFree(fsa_t *fsa)
{
	size_t count = 0;
	size_t next_free_address = fsa->next_free;
	fsa_block_header_t *header = {0};
	
	assert(fsa);
	
	while (0 != next_free_address)
	{
		++count;
		header = (fsa_block_header_t *)((char *)fsa + next_free_address);
		next_free_address = header->offset;
	}
	
	return count;
}

static void SwapOffsetIMP(size_t *offset_of_block, size_t *offset_of_struct)
{
    size_t temp = *offset_of_block;
    *offset_of_block = *offset_of_struct;
    *offset_of_struct = temp;
}
