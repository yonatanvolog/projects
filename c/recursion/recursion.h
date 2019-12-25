/****************************************************************************
*                                                                           *
*                               OL79 - 22/12/19                             *
*                                                                           *	
*                                Recursion                                  *
*                                                                           *
*****************************************************************************/

#ifndef ILRD_RECURSION_H
#define ILRD_RECURSION_H

#include <stddef.h>

#include "stack.h"
#include "slist.h"


typedef int (*cmp_func_t)(void *data1, void *data2);
  
/* The function returns the value of the nth element in fibonacci series via 
   standard iteration, starting at n(0) = 0, n(1) = 1, n(2) = 1, n(3) = 2... */
unsigned long IterativeFibonacci(unsigned int n);

/* The function returns the value of the nth element in fibonacci series via
   recursion */
unsigned long RecursiveFibonacci(unsigned int n);

/* The function returns the length of the string */
size_t RecStrlen(const char *str);

/* The function compares two strings (pointed by str1 and str2) */
int RecStrcmp(const char *str1, const char *str2);

/* The function copies the string pointed by src to dest */
char *RecStrcpy(char *dest, const char *src);

/* The function appends the string pointed to by src to the end of the string
   pointed to by dest */
char *RecStrcat(char *dest, const char *src);

/* function finds the first occurrence of the substring needle in the string
   haystack. The terminating '\0' characters are not compared */
char *RecStrstr(const char *haystack, const char *needle);

/* The function reverses the node order, starting from head untill null */
sl_node_t *RecFlip(sl_node_t *head);

/* The function sorts the stack according to the sorting function.
*	The function receives a compare function, supplied by the user,
*	which should return 1 if data1 is before data2, 0 if they are equal 
*	and -1 if data2 is before data1. */ 
stack_t *RecSortStack(stack_t *stack, size_t element_size, cmp_func_t func);

#endif	
