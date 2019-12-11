/**********************************************************************
 *	This code is representing my functions of VSA RC2		          *
 *	Lab: OL-79                                		                  *
 *	Author: Yonatan Vologdin 										  *
 *	Reviewer: Tamir Dayan        		  							  *
 *	Date: 03.12.19													  *
 **********************************************************************/
#include <stdio.h>			/*NULL*/
#include <assert.h>			/*assert*/

#include "vsa.h"			/*vsa*/
#include "yoni_utils.h"		/*utils*/

#define HEADER_SIZE 		sizeof(vsa_block_header_t)
#define MAGIC_NUM 			12345
#define BLOCK_SIZE(var)		(((vsa_t *)(var))->block_size)
#define MAGIC_NUMBER(var)	(((vsa_t *)(var))->magic_number)
#define CHECK_PADDING(var)  ((0 == var) ? 0 : (WORD_SIZE - var))

typedef struct block_header
{
	ssize_t block_size;
	#ifndef NDEBUG
    size_t magic_number;
    #endif
}vsa_block_header_t;

static void CreateHeaderIMP(void *address, ssize_t block_size, size_t magic_number);
static vsa_t *FirstLargeEnoughChunkIMP(vsa_t *vsa, size_t block_size);
static void JumpToNextHeaderIMP(char **runner);

vsa_t *VSAInit(void *memory_pool, size_t pool_size)
{
	char *init_runner = NULL;

	assert(memory_pool);
	
	if (0 == pool_size)
	{
		return NULL;
	}

	pool_size = pool_size - (pool_size % WORD_SIZE);
	init_runner = (char *)memory_pool + pool_size - HEADER_SIZE;
	CreateHeaderIMP(init_runner, 0, MAGIC_NUM);

	pool_size -= 2*HEADER_SIZE;
	init_runner = (char *)memory_pool;
	CreateHeaderIMP(init_runner, (-1)*pool_size, MAGIC_NUM);

	return (vsa_t *)init_runner;
}

static void CreateHeaderIMP(void *address, ssize_t block_size, size_t magic_number)
{
	BLOCK_SIZE(address) = block_size;
	#ifndef NDEBUG
	MAGIC_NUMBER(address) = magic_number;
    #endif
	UNUSE(magic_number);
}

void *VSAAlloc(vsa_t *vsa, size_t block_size)
{
	char *to_allocate = NULL;
	size_t user_size = block_size + CHECK_PADDING(block_size % WORD_SIZE);
	size_t actual_size = user_size + HEADER_SIZE;
	
	assert(vsa);
	
	if (0 == block_size)
	{
		return NULL;
	}

	if (user_size > VSALargestChunk(vsa))
	{
		return NULL;
	}

	to_allocate = (char *)FirstLargeEnoughChunkIMP(vsa, user_size);

	if (((-1)*BLOCK_SIZE(to_allocate) - user_size) > (HEADER_SIZE + WORD_SIZE))
	{	
		CreateHeaderIMP(to_allocate + actual_size,
						(-1)*((-1)*BLOCK_SIZE(to_allocate) - actual_size),
						MAGIC_NUM);

		BLOCK_SIZE(to_allocate) = user_size;
	}
	
	else
	{
		BLOCK_SIZE(to_allocate) *= (-1);
	}

	return (to_allocate + HEADER_SIZE);
}

static vsa_t *FirstLargeEnoughChunkIMP(vsa_t *vsa, size_t block_size)
{
	char *first = (char *)vsa;
	char *runner = (char *)vsa;

	ssize_t header_count = -1;
	size_t free_bytes_in_block = 0;
	size_t total_free_bytes = 0;

	while (total_free_bytes < block_size)
	{
		if (0 > BLOCK_SIZE(runner))
		{	
			++header_count;
			free_bytes_in_block += ABS(BLOCK_SIZE(runner));
			total_free_bytes = header_count * HEADER_SIZE + free_bytes_in_block;
			JumpToNextHeaderIMP(&runner);
		}

		else 
		{	
			if(header_count >= 0)
			{
				total_free_bytes = header_count * HEADER_SIZE + free_bytes_in_block;
				BLOCK_SIZE(first) = (-1)*total_free_bytes;
			}

			JumpToNextHeaderIMP(&runner);
			first = runner;
			header_count = -1;
			free_bytes_in_block = 0;
		}
	}

	BLOCK_SIZE(first) = (-1)*total_free_bytes;

	return (vsa_t *)first;
}

static void JumpToNextHeaderIMP(char **runner)
{
	*runner = *runner + HEADER_SIZE + ABS(BLOCK_SIZE(*runner));
}

void VSAFree(void *allocated_address)
{
	char *block_to_free = (char *)allocated_address;
	int magic_num_flag = 1;	

	if (NULL != allocated_address)
	{
		block_to_free -= HEADER_SIZE;
		#ifndef NDEBUG
		magic_num_flag = (MAGIC_NUM == MAGIC_NUMBER(block_to_free));
		#endif

		if (1 == magic_num_flag)
		{
			BLOCK_SIZE(block_to_free) *= (-1);
		}
	}
}

size_t VSALargestChunk(vsa_t *vsa)
{
	char *runner = (char *)vsa;
	ssize_t max_in_bytes = 0;
	ssize_t tmp_in_bytes = 0;

	assert(vsa);

	while (0 != BLOCK_SIZE(runner))
	{
		if (0 > BLOCK_SIZE(runner))
		{	
			tmp_in_bytes += (ABS(BLOCK_SIZE(runner)) + HEADER_SIZE);
		}

		else
		{
			max_in_bytes = MAX(tmp_in_bytes, max_in_bytes);
			tmp_in_bytes = 0;
		}
		JumpToNextHeaderIMP(&runner);
	}

	max_in_bytes = MAX(tmp_in_bytes, max_in_bytes);
	max_in_bytes -= HEADER_SIZE;

	if (0 > max_in_bytes)
	{
		return 0;
	}

	return max_in_bytes;
}
