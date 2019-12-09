



/****************************************************************************
*                                                                           *
*                               OL79 - 13/11/19                             *
*                                                                           *	
*                                sorted list                                *
*                                                                           *
*****************************************************************************/

#ifndef ILRD_SORTED_LIST_H
#define ILRD_SORTED_LIST_H

#include <stddef.h>
#include "dllist.h"

typedef struct srt_list srt_list_t;
typedef struct srt_iterator srt_iter_t ;
/* data1 is the new data. data2 is the data to compare (from the list).
   param can be used for a deeper compare function */
typedef int (*is_before_t)(const void *new_data, const void *src_data, void *param);
typedef int (*is_match_func_t)(const void *data, const void *param);
typedef int (*action_func_t)(void *data, void *param);

struct srt_iterator
{
	dll_iter_t dll_iterator;
};

/*	Function creates a sorted list struct.
	Receives the sort function
	returns pointer to the sorted list struct 
	if the function fails, it returns NULL
	user must supply a valid sort function*/
srt_list_t *SrtListCreate(void *param, is_before_t is_before);

/*
	The function destroys all nodes in list and frees all memory allocations
	The list cannot be NULL
*/
void SrtListDestroy(srt_list_t *list);

/*The function inserts a new node to the correct sorted place
	according to the sorting function.
	The function returns an iterator to the new node.
	if the function fails, it returns an iterator to the end of the list 
	The list cannot be NULL */
srt_iter_t SrtListInsert(void *data, srt_list_t *list);

/*	The function pops the front node.
	The function returns a pointer to the data that was in the node.
	The function frees memory allocation of the node.
	If the list is empty, behavior is undifined
	The list cannot be NULL */
void *SrtListPopFront(srt_list_t *list);

/*	The function pops the back node.
	The function returns a pointer to the data that was in the node.
	The function frees memory allocation of the node.
	If the list is empty, behavior is undifined
	The list cannot be NULL */
void *SrtListPopBack(srt_list_t *list);

/* Function removes iterator and returns an iterator of the next node
   The function frees memory allocation of the node
   After SrtListRemove call, the data will be lost! */
srt_iter_t SrtListRemove(srt_iter_t iterator);

/*	The function receives a sorted list.
	The Function returns the size of the list
	The list cannot be NULL */
size_t SrtListSize(const srt_list_t *list);

/*	The function receives sorted list.
	The function returns 1 if the list is Empty and zero if not
	The list cannot be NULL */
int SrtListIsEmpty(const srt_list_t *list);

/*	The function loops over every node, from begin (inclusive) to end (exclusive). 
	The function SrtListFind uses the is_before function to find a match
 *	If SrtListFind found a match, The SrtListFind returns a pointer to the matched node,
 *	else - it will return an iterator to the iterator end.   
	The list cannot be NULL */
srt_iter_t SrtListFind(srt_iter_t begin, srt_iter_t end, 
					   const void *data, srt_list_t *list);

/*	The function loops over every node, from begin (inclusive) to end (exclusive),
 *	and passes it, alongside param, to the function that is pointed by match_func. 
 *	The match_func function should look for param in the node that is passed to it. 
 *	If match_func finds a match, it should return 1.
 *	If match_func found a match, The SrtListFindIf returns a pointer to the matched node.
 *	else - it will return an iterator to the iterator end */
srt_iter_t SrtListFindIf(srt_iter_t begin, srt_iter_t end, 
						 const void *param, is_match_func_t match_func);

/*	The function loops over every node, from from begin (inclusive) to end (exclusive),
 *	and passes it along side param, to the function that is pointed by action_func. 
 *	the action_func function can do whatever you want, but it should return 1 if it fails.
 *	If the DLListForEach functions recieves 1 from action_func, 
 *	it will stop the foreach and will return 0 passes, 1 if fails */
int SrtListForEach(srt_iter_t begin, srt_iter_t end, void *param, action_func_t action_func);

/*	Function merges and sorts between two lists. 
	It receives 2 lists: source list and destination list to which to connect it. 
	Function returns void.
	src list and dest list must be of different lists.
	The source list is destroyed after the process.
	The lists cannot be NULL */
void SrtListMerge(srt_list_t *src_list, srt_list_t *dest_list);

/*	Function receives list. Function returns iterator to start of list inclusive
	The lists cannot be NULL */
srt_iter_t SrtListBegin(srt_list_t *list);

/*	Function receives list. Function returns iterator to end of list exclusive 
	The lists cannot be NULL */
srt_iter_t SrtListEnd(srt_list_t *list);

/*	Function receives iterator. Function returns next iterator
	If iterator is the end node this will result in undifined behavior */
srt_iter_t SrtListNext(srt_iter_t iterator);

/*	Function receives iterator. Function returns previous iterator
	If iterator is the start node this will result in undifined behavior */
srt_iter_t SrtListPrev(srt_iter_t iterator);

/*	Function receives iterator. Function returns data located inside */
void *SrtListGetData(srt_iter_t iterator);

/*	Function receives two iterators. Function returns 1 if the are equal 
	and 0 if not */
int SrtListIsSameIterator(srt_iter_t iterator1, srt_iter_t iterator2);

#endif
