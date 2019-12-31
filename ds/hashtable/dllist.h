/****************************************************************************
*                                                                           *
*                               OL79 - 11/11/19                             *
*                                                                           *	
*                                   DLList                                   *
*                                                                           *
*****************************************************************************/

#ifndef ILRD_DLLIST_H
#define ILRD_DLLIST_H

#include <stddef.h>

typedef struct node dll_node_t;
typedef dll_node_t *dll_iter_t;
typedef struct dl_list dl_list_t;
typedef int (*find_ptr)(const void *data, const void *param);
typedef int (*for_each_ptr)(void *data, void *param);

/* Function creates a list struct.
   returns pointer to list struct 
   if fails returns NULL
   TC = O(1)
   */
dl_list_t *DLListCreate(void);

/*
* The function destroys all nodes in list
* Frees all memory allocations
   TC = O(1)
*/
void DLListDestroy(dl_list_t *list);

/*Function adds node to the end of the list. 
  Returns node (iterator) to new added node
  if fails returns end iterator (dllistEnd) 
  TC = O(1)
  */
dll_iter_t DLListPushBack(dl_list_t *list, void *data);

/*Function adds node to the front of the list. 
  Returns node (iterator) to new added node
  if fails returns end iterator (dllistEnd) 
  TC = O(1)
  */
dll_iter_t DLListPushFront(dl_list_t *list, void *data);

/*The function inserts a new node before iterator.
 * The function returns an iterator to the new node.
   If fails returns end iterator (dllistEnd) 
   TC = O(1)
   */
dll_iter_t DLListInsert(void *data, dll_iter_t iterator, dl_list_t *list);

/* The function pops the front node.
   The function returns pointer to the data that was in the node.
   function frees the node.
   if list is empty behavior undifined 
   TC = O(1)
   */
void *DLListPopFront(dl_list_t *list);

/* The function pops the back node.
 * The function return pointer to the data that was in the node.
   function frees the node.
*  If list is empty pop will result in undifined behavior
   TC = O(1)
 */
void *DLListPopBack(dl_list_t *list);

/* Function removes iterator and returns iterrator of the next node
*  function frees the node.
*  If list is empty pop will result in undifined behavior
*  TC = O(1)
*/
dll_iter_t DLListRemove(dll_iter_t iterator);

/*Function merges between two lists. It receives a section of 
one list begin(inclusive) and end (exclusive) and destination to which to connect it. 
Function returns destination.
The user must verify the that begin -> ... -> end
begin and dest must be of different lists.
After splicing it is the user's responsibility to destroy what was left 
of the old list
TC = O(1)
*/
dll_iter_t DLListSplice(dll_iter_t s_begin, dll_iter_t s_end, dll_iter_t dest);

/* Function receives list.
   Function returns size of list 
   TC = O(n)
   */ 
size_t DLListSize(const dl_list_t *list);

/* Function receives list.
   Function returns 1 if Empty and 0 if not 
   TC = O(n)
   */ 
int DLListIsEmpty(const dl_list_t *list);

/* The function loops over every node, from *begin until end (exclusive), and passes it,
 * alongside param, to the function that is pointed by ptr. 
 * The ptr function should look for param in the node that is passed to it. 
 * If ptr finds a match, it should return 1.
 * If ptr found a match, The DLListFind returns a pointer to the matched node.
 * else - it will return end iterator 
  If fails returns end 
  TC = O(n)
  */
dll_iter_t DLListFind(dll_iter_t begin, dll_iter_t end, 
					  const void *param, find_ptr ptr);

/* The function loops over every node, from *begin to end exclusive, and passes it,
 * along side param, to the function that is pointed by ptr. the ptr function
 * can do whatever you want, but it should return 1 if it fails.
 * If the DLListForEach functions recieves 1 from ptr, 
 * it will stop the foreach and will return 0 passes, 1 if fails 
 TC = O(n)
 */
int DLListForEach(dll_iter_t begin, dll_iter_t end, 
				  void *param, for_each_ptr ptr);

/* Function receives list. Function returns iterator to start of list inclusive
TC = O(1)
*/
dll_iter_t DLListBegin(const dl_list_t *list);

/* Function receives list. Function returns iterator to end of list exclusive 
TC = O(1)
*/
dll_iter_t DLListEnd(const dl_list_t *list);

/* Function receives iterator. Function returns next iterator
   If iterator is the end node this will result in undifined behavior 
   TC = O(1)
   */
dll_iter_t DLListNext(dll_iter_t iterator);

/* Function receives iterator. Function returns previous iterator
If iterator is the start node this will result in undifined behavior 
TC = O(1)
*/
dll_iter_t DLListPrev(dll_iter_t iterator);

/* Function receives iterator. Function returns data located inside 
TC = O(1)
*/
void *DLListGetData(dll_iter_t iterator);

/* Function receives two iterators. Function returns 1 if the are equal 
and 0 if not 
TC = O(1)
*/
int DLListIsSameIterator(dll_iter_t iterator1, dll_iter_t iterator2);

#endif
