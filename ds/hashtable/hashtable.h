/****************************************************************************
*                                                                           *
*                               OL79 - 26/12/19                             *
*                                                                           *
*                                 Hash Table                                *
*                                                                           *
*****************************************************************************/

#ifndef ILRD_HASHTABLE_H
#define ILRD_HASHTABLE_H

#include <stddef.h> /* size_t */

typedef struct hash hash_table_t;
typedef size_t (*hash_func_t)(const void *key);
typedef int (*action_func_t)(void *data, void *action_param);
typedef int (*is_match_t)(const void *current_data,
                          const void *data_to_compare);

/* * * * * * MOVE TO SOURCE FILE !! * * * * 
struct hash
{
 hash_func_t hash_func;
 is_match_t match_func;
 size_t num_of_buckets;
 dl_list_t **table;
};
* * * * * * * * * * * * * * * * * * * * */

/* Function creates a Hash Table data structure.
 * Receives a hash function pointer, match function pointer and num_of_buckets.
 *
 * The hash_func and match_func can't be NULL.
 * num_of_buckets should be greater than 0.
 * hash_func should return index of the table.
 * match_func should return 1 if data was found, 0 otherwise.
 * Return Values:
 *                - returns pointer to the Hash Table.
 *                - if the function fails, it returns NULL. */
hash_table_t *HashCreate(size_t num_of_buckets,
                         hash_func_t hash_func,
                         is_match_t match_func);

/* The function destroys the Hash Table and frees all memory allocations. */
void HashDestroy(hash_table_t *hash_table);

/* The function inserts new data to the table according to the hash function.
 * If the data already exists in the hash table, it will insert duplicate data.
 * The pointer must point to a valid hash table. 
 * Function will insert the new data according to the hash function.
 * Return Value: returns 0 if success, non-zero value otherwise. 
 * If hash_table is Empty, function will result in undefined behaviour. */
int HashInsert(hash_table_t *hash_table, void *data);

/* Function removes the data that is matching with data sent.
 * After HashRemove call, the access data will be lost!
 * The hash_table pointer must point to a valid hash_table. 
 * If data is not found, then function is terminated */
void HashRemove(hash_table_t *hash_table, const void *data);

/* The function looks up for data in the hash_table.
 * The function loops over entire hash_table until data is found.
 * Return Value: If finds a match, it will return pointer to data.
 *               else - it will return NULL.
 * The pointer must point to a valid Hash Table. */
void *HashFind(const hash_table_t *hash_table, const void *data);

/* The function receives hash table pointer.
 * Return Values: 1 if the table contains no elements, 0 otherwise.
 * The pointer must point to a valid hash table. */ 
int HashIsEmpty(const hash_table_t *hash_table);

/* The function receives a pointer to the hash_table.
 * The Function returns the size of the hash_table.
 * The pointer must point to a valid hash table. */
size_t HashSize(const hash_table_t *hash_table);

/* The function loops over hash_table, and for every node it will pass the data,
 * alongside param, to the action_func function. 
 * the action_func function can do whatever you want, 
 * but it should return a non - zero value if it fails.
 * If the HashForEach function recieves non - zero value from action_func, 
 * it will stop with a fail.
 * the HashForEach will return 0 if completed its task, 
 * else the non - zero value it received from the action_func function */
int HashForEach(hash_table_t *hash_table,
                action_func_t action_func,
                void *action_param);

/* The function receives a pointer to the hash_table.
*  The function returns the Load Factor (num of elements / num of buckets). */
double HashGetLoadFactor(const hash_table_t *hash_table);

/* The function receives a pointer to the hash_table.
*  The function returns the Standard Deviation. */
double HashGetStandardDeviation(const hash_table_t *hash_table);

#endif
