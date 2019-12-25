/**********************************************************************
 *	This code is representing my functions of recursion RC2           *
 *	Lab: OL-79                                		                  *
 *	Author: Yonatan Vologdin 										  *
 *	Reviewer: Chen Rubin	 	  							     	  *
 *	Date: 24.12.19													  *
 **********************************************************************/
#include <string.h>	/*strlen*/
#include <assert.h>	/*assert*/
#include <stdlib.h>	/*malloc*/

#include "slist.h"
#include "stack.h"
#include "recursion.h"
#include "yoni_utils.h"

unsigned long IterativeFibonacci(unsigned int n)
{
	size_t i = 0;
	unsigned long prev_prev = 0;
	unsigned long prev = 1;
	unsigned long current = 0;
	
	for (i = 0; i < n; ++i)
	{
		prev_prev = prev;
		prev = current;
		current = prev_prev + prev;
	}
	
	return current;
}

unsigned long RecursiveFibonacci(unsigned int n)
{	
	if (n <= 1)
	{
		return n;		
	}

	return (RecursiveFibonacci(n - 2) + RecursiveFibonacci(n - 1));
}

size_t RecStrlen(const char *str)
{
	assert(str);	
	
	if ('\0' == *str)
	{
		return 0;
	}

	return (RecStrlen(str+1) + 1);
}

int RecStrcmp(const char *str1, const char *str2)
{
	assert(str1);
	assert(str2);

	if ((*str1 != *str2) || (('\0' == *str1) || ('\0' == *str2)))
	{
		return (*str1 - *str2);
	}
	
	return (RecStrcmp(str1 + 1, str2 + 1));
}

char *RecStrcpy(char *dest, const char *src)
{
	assert(dest);
	assert(src);

	if ('\0' == *src)
	{
		*dest = *src;
		
		return dest;
	}
	*dest = *src;
	RecStrcpy(dest + 1, src + 1);

	return dest;
}

char *RecStrcat(char *dest, const char *src)
{
	assert(dest);
	assert(src);

	if ('\0' == *src)
	{
		return dest;
	}
	*(dest + strlen(dest)) = *src;

	return RecStrcat(dest, src + 1);
}

char *RecStrstr(const char *haystack, const char *needle)
{
	assert(haystack);
	assert(needle);

	if(0 == strncmp(haystack, needle, strlen(needle)))
	{
		return (char *)haystack;
	}
	
	if ('\0' == *haystack)
	{
		return NULL;
	}

	return RecStrstr(haystack + 1, needle);
}

sl_node_t *RecFlip(sl_node_t *head)
{
	sl_node_t *new_head = NULL;

	assert(head);
	
	if (NULL == head->next)
	{
		return head;
	}
	new_head = RecFlip(head->next);

	head->next->next = head;	
	head->next = NULL;	

	return new_head;
}

static void StackSwapAndInsertIMP(stack_t *stack, size_t element_size,
										 void *current_val, void *tmp)
{
    memcpy(tmp, current_val, element_size);
    memcpy(current_val, StackPeek(stack), element_size);
    StackPop(stack);
    StackPush(stack, tmp);
}

stack_t *RecSortStack(stack_t *stack, size_t element_size, cmp_func_t func)
{
    void *current_val = NULL;
    void *tmp = NULL;

    assert(stack);
    assert(func);

    current_val = (void *)malloc(element_size);
    if(NULL == current_val)
    {
        return NULL;
    }

    tmp = (void *)malloc(element_size);
    if(NULL == tmp)
    {
		free(current_val);

        return NULL;
    }

	memcpy(current_val, StackPeek(stack), element_size);
	StackPop(stack);
    
    if (StackIsEmpty(stack))
    {
        StackPush(stack, current_val);
        free(current_val);
        free(tmp);

        return stack;
    }

    RecSortStack(stack, element_size, func);

    if (1 == func(current_val, StackPeek(stack)))
    {    
        StackSwapAndInsertIMP(stack, element_size, current_val, tmp);
        RecSortStack(stack, element_size, func);
    }

    StackPush(stack, current_val);
        
    free(current_val);
    free(tmp);

    return stack;
}
