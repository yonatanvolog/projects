/**********************************************************************
 *	This code is representing my tests of FSA RC2			       	  *
 *	Lab: OL-79                                		                  *
 *	Author: Yonatan Vologdin 										  *
 *	Reviewer: Eyal Finkelshtein  		  							  *
 *	Date: 29.11.19													  *
 **********************************************************************/
#include <stdio.h>		/*printf*/
#include <stdlib.h>		/*malloc*/

#include "fsa.h"		/*fsa*/
#include "../../yoni_utils.h"

#define FSA_SIZE	sizeof(fsa_t)
#define HEADER_SIZE sizeof(fsa_block_header_t)

struct fsa
{
	size_t next_free;
	size_t block_size;
};

typedef struct block_header
{
	size_t offset;
}fsa_block_header_t;

int SuggestedSize();
int InitTest();
int AllocTest();
int FreeTest();
int CountFreeTest();

int main()
{
	RUN_SEQUENCE("SuggestedSize", SuggestedSize());
	RUN_SEQUENCE("InitTest", InitTest());
	RUN_SEQUENCE("AllocTest", AllocTest());
	RUN_SEQUENCE("FreeTest", FreeTest());
	RUN_SEQUENCE("CountFreeTest", CountFreeTest());
	
	return 0;
}

int SuggestedSize()
{
	/*fsa + num_of_blocks*(header+block_size+padding)*/
	/*16 + 0*(8+10+6)*/
	RUN_TEST("SuggestedSizeTest1", 16 == FSASuggestedSize(0, 10));
	/*16 + 10*(8+0+8)*/
	RUN_TEST("SuggestedSizeTest2", 176 == FSASuggestedSize(10, 0));
	/*16 + 3*(8+3+5)*/
	RUN_TEST("SuggestedSizeTest3", 64 == FSASuggestedSize(3, 3));
	/*16 + 10*(8+10+6)*/
	RUN_TEST("SuggestedSizeTest4", 256 == FSASuggestedSize(10, 10));

	return 0;
}

int InitTest()
{
	size_t pool_size = 64;
	size_t block_size = 3;
	size_t actual_block_size = HEADER_SIZE + block_size + 5;

	fsa_t *fsa = NULL;
	char *memory_pool = (char *)malloc(pool_size);
	fsa = FSAInit(memory_pool, pool_size, block_size);
	
	/*management struct: next free = FSA_SIZE*/
	RUN_TEST("InitFSATest1", FSA_SIZE == *(size_t *)memory_pool);

	/*management struct: block size = actual_block_size*/
	memory_pool += 8;
	RUN_TEST("InitFSATest2", actual_block_size == *(size_t *)memory_pool);	

	memory_pool += 8;
	RUN_TEST("HeaderTest1", fsa->next_free + actual_block_size * 1 == *(size_t *)memory_pool);

	memory_pool += actual_block_size;
	RUN_TEST("HeaderTest2", fsa->next_free + actual_block_size * 2 == *(size_t *)memory_pool);

	/*header: offset to next free = 0 (last block)*/
	memory_pool += actual_block_size;
	RUN_TEST("HeaderTest3", 0 == *(size_t *)memory_pool);

	free(fsa);
	
	return 0;
}

int AllocTest()
{
	char *writable_address1 = NULL;
	char *writable_address2 = NULL;
	char *writable_address3 = NULL;

	size_t pool_size = 64;
	size_t block_size = 3;
	size_t block_padding = 5;

	char *memory_pool = (char *)malloc(pool_size);
	fsa_t *fsa = NULL;

	fsa = FSAInit(memory_pool, pool_size, block_size);

	writable_address1 = FSAAlloc(fsa);
	writable_address2 = FSAAlloc(fsa);
	writable_address3 = FSAAlloc(fsa);

	/*fsa->next_free*/
	RUN_TEST("NextFreeTest", 0 == *(size_t *)memory_pool);

	/*block_header->offset*/
	memory_pool += FSA_SIZE;
	RUN_TEST("OffsetTest1", 16 == *(size_t *)memory_pool);
	/*allocated address test*/
	memory_pool += HEADER_SIZE;
	RUN_TEST("AddressTest1", writable_address1 == memory_pool);
	
	memory_pool += (block_size + block_padding);
	RUN_TEST("OffsetTest2", 32 == *(size_t *)memory_pool);
	memory_pool += HEADER_SIZE;
	RUN_TEST("AddressTest2", writable_address2 == memory_pool);

	memory_pool += (block_size + block_padding);
	RUN_TEST("OffsetTest3", 48 == *(size_t *)memory_pool);
	memory_pool += HEADER_SIZE;
	RUN_TEST("AddressTest3", writable_address3 == memory_pool);

	free(fsa);
	
	return 0;
}

int FreeTest()
{
	char *writable_address1 = NULL;
	char *writable_address2 = NULL;
	char *writable_address3 = NULL;

	size_t pool_size = 64;
	size_t block_size = 3;
	size_t actual_block_size = HEADER_SIZE + block_size + 5;

	char *memory_pool = (char *)malloc(pool_size);
	fsa_t *fsa = NULL;

	fsa = FSAInit(memory_pool, pool_size, block_size);

	writable_address1 = FSAAlloc(fsa);
	writable_address2 = FSAAlloc(fsa);
	writable_address3 = FSAAlloc(fsa);

	FSAFree(writable_address3);
	FSAFree(writable_address2);
	FSAFree(writable_address1);

	/*management struct: next free = FSA_SIZE*/
	RUN_TEST("FreeTest1", FSA_SIZE == *(size_t *)memory_pool);

	/*management struct: block size = actual_block_size*/
	memory_pool += 8;
	RUN_TEST("FreeTest2", actual_block_size == *(size_t *)memory_pool);	

	memory_pool += 8;
	RUN_TEST("FreeTest3", fsa->next_free + actual_block_size * 1 == *(size_t *)memory_pool);

	memory_pool += actual_block_size;
	RUN_TEST("FreeTest4", fsa->next_free + actual_block_size * 2 == *(size_t *)memory_pool);

	/*header: offset to next free = 0 (last block)*/
	memory_pool += actual_block_size;
	RUN_TEST("FreeTest5", 0 == *(size_t *)memory_pool);

	free(fsa);
	
	return 0;
}

int CountFreeTest()
{
	char *writable_address1 = NULL;
	char *writable_address2 = NULL;
	char *writable_address3 = NULL;

	size_t pool_size = 64;
	size_t block_size = 3;

	char *memory_pool = (char *)malloc(pool_size);
	fsa_t *fsa = NULL;

	fsa = FSAInit(memory_pool, pool_size, block_size);

	RUN_TEST("CountTestAlloc1", 3 == FSACountFree(fsa));
	writable_address1 = FSAAlloc(fsa);
	RUN_TEST("CountTestAlloc2", 2 == FSACountFree(fsa));
	writable_address2 = FSAAlloc(fsa);
	RUN_TEST("CountTestAlloc3", 1 == FSACountFree(fsa));
	writable_address3 = FSAAlloc(fsa);
	RUN_TEST("CountTestAlloc4", 0 == FSACountFree(fsa));

	FSAFree(writable_address3);
	RUN_TEST("CountTestFree1\t", 1 == FSACountFree(fsa));
	FSAFree(writable_address2);
	RUN_TEST("CountTestFree2\t", 2 == FSACountFree(fsa));
	FSAFree(writable_address1);
	RUN_TEST("CountTestFree3\t", 3 == FSACountFree(fsa));

	free(fsa);

	return 0;
}
