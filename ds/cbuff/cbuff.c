/**********************************************************************
 *	This code is representing my functions of DS_CBuff RC2            *
 *	Lab: OL-79                                		                  *
 *	Author: Yonatan Vologdin 										  *
 *	Reviewer: Eyal Finkelshtein   								      *
 *	Date: 11.11.19													  *
 **********************************************************************/
#include <stdlib.h>	/*malloc*/
#include <string.h> /*memcpy*/
#include <assert.h>	/*assert*/ 

#include "cbuff.h"	/*circular buffer data structure*/

#define SIZE_OF_CB (sizeof(size_t)+sizeof(char*)+sizeof(char*)+sizeof(char))
#define MIN(value1, value2) (((value1) < (value2)) ? (value1) : (value2))
#define PTR_TO_LAST_ELEMENT (cbuff->begin + cbuff->capacity)
 
struct CB
{
	size_t capacity;
	char *r_ptr;
	char *w_ptr;
	char begin[1];
};

cbuff_t *CBuffCreate(size_t capacity)
{
	cbuff_t *new_buffer = (cbuff_t *)malloc(SIZE_OF_CB + capacity);
	if (NULL == new_buffer)
	{
		return NULL;
	}
	
	new_buffer->capacity = capacity;
	new_buffer->r_ptr = new_buffer->begin;
	new_buffer->w_ptr = new_buffer->begin;
	
	return new_buffer;
}

void CBuffDestroy(cbuff_t *cbuff)
{	
	assert(NULL != cbuff);

	free(cbuff);
	cbuff = NULL;
}

int CBuffIsEmpty(const cbuff_t *cbuff)
{
	assert(NULL != cbuff);
	
	return ((cbuff->r_ptr == cbuff->w_ptr) ? 1 : 0);
}

size_t CBuffCapacity(const cbuff_t *cbuff)
{
	assert(NULL != cbuff);
	
	return cbuff->capacity;
}

size_t CBuffSpaceLeft(const cbuff_t *cbuff)
{
	assert(NULL != cbuff);

	if (cbuff->w_ptr > cbuff->r_ptr)
	{
		return (cbuff->capacity - (cbuff->w_ptr - cbuff->r_ptr));
	}
	
	else if (cbuff->r_ptr > cbuff->w_ptr)
	{
		return (cbuff->r_ptr - cbuff->w_ptr);
	}
	
	return cbuff->capacity;
}

ssize_t CBuffWrite(cbuff_t *cbuff, const void *src, size_t nbytes)
{
	char *src_runner = (char*)src;
	size_t space_till_end = 0;
	nbytes = MIN(nbytes, CBuffSpaceLeft(cbuff));
	
	assert(NULL != cbuff);
	assert(NULL != src);
	
	space_till_end = PTR_TO_LAST_ELEMENT - cbuff->w_ptr;
	
	if (space_till_end >= nbytes)
	{	
		memcpy(cbuff->w_ptr, src_runner, nbytes);
		cbuff->w_ptr += nbytes;
	}
	
	else /*nbytes > space_till_end*/
	{
		memcpy(cbuff->w_ptr, src_runner, space_till_end);
		cbuff->w_ptr = cbuff->begin;
		src_runner += space_till_end;
		memcpy(cbuff->w_ptr, src_runner, (nbytes - space_till_end));
		cbuff->w_ptr += (nbytes - space_till_end);
	}

	return (ssize_t)nbytes;	
}

ssize_t CBuffRead(cbuff_t *cbuff, void *dest, size_t nbytes)
{
	char *dest_runner = (char*)dest;		
	size_t space_till_end = 0;
	nbytes = MIN(nbytes, (CBuffCapacity(cbuff) - CBuffSpaceLeft(cbuff)));

	assert(NULL != cbuff);
	assert(NULL != dest);

	space_till_end = PTR_TO_LAST_ELEMENT - cbuff->r_ptr;
	
	if (space_till_end >= nbytes)
	{	
		memcpy(dest_runner, cbuff->r_ptr, nbytes);
		cbuff->r_ptr += nbytes;
	}
	
	else /*nbytes > space_till_end*/
	{
		memcpy(dest_runner, cbuff->r_ptr, space_till_end);
		cbuff->r_ptr = cbuff->begin;
		dest_runner += space_till_end;
		memcpy(dest_runner, cbuff->r_ptr, (nbytes - space_till_end));
		cbuff->r_ptr += (nbytes - space_till_end);
	}

	return (ssize_t)nbytes;	
}
