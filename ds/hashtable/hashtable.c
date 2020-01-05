/**********************************************************************
 *	This code is representing my functions of Hashtable  RC2	      *
 *	Lab: OL-79                                		                  *
 *	Author: Yonatan Vologdin 										  *
 *	Reviewer: Dvir Breitbart 	 	  						     	  *
 *	Date: 29.12.19													  *
 **********************************************************************/
#include <stdlib.h>	/*malloc, free*/
#include <assert.h>	/*assert*/
#include <math.h>	/*pow, sqrt*/

#include "dllist.h"
#include "hashtable.h"
#include "../../yoni_utils.h"

struct hash
{
 hash_func_t hash_func;
 is_match_t match_func;
 size_t num_of_buckets;
 dl_list_t **table;
};

typedef struct variance_s
{
 	double mean;
 	double sum_of_sq_diff;
}variance_s;

static void DestoryAllPreviousDLListsIMP(dl_list_t **table_iter, size_t last);
static dl_list_t *GetBucketIMP(const hash_table_t *hash_table, const void *data);

hash_table_t *HashCreate(size_t num_of_buckets,
                         hash_func_t hash_func,
                         is_match_t match_func)
{
	size_t i = 0;
	size_t table_size = 0;	
    dl_list_t **table_iter = NULL;
	hash_table_t *new_hashtable = NULL;
	
	assert(hash_func);
	assert(match_func);

	new_hashtable =(hash_table_t *)malloc(sizeof(hash_table_t));
	if (NULL == new_hashtable)
	{
		return NULL;
	}
   
    table_size = sizeof(dl_list_t *) * num_of_buckets;
    new_hashtable->table = (dl_list_t **)malloc(table_size);
    if (NULL == new_hashtable->table)
    {
        free(new_hashtable);
        
        return NULL;
    }
    table_iter = new_hashtable->table;	
    for (i = 0; i < num_of_buckets; ++i)
    {   
        *(table_iter + i) = DLListCreate();
        if (NULL == table_iter)
        {
			DestoryAllPreviousDLListsIMP(table_iter, i);
            free(new_hashtable);
        
            return NULL;
        }
    }
    
    new_hashtable->hash_func = hash_func;
    new_hashtable->match_func = match_func;
    new_hashtable->num_of_buckets = num_of_buckets;

    return new_hashtable;
}

static void DestoryAllPreviousDLListsIMP(dl_list_t **table_iter, size_t last)
{
	size_t i = 0;
	
	for (i = 0; i <= last; ++i)
	{
		DLListDestroy(*(table_iter + i));
	}
}


void HashDestroy(hash_table_t *hash_table)
{
	size_t i = 0;
    dl_list_t **table_iter = NULL;

	assert(hash_table);
	
	table_iter = hash_table->table;
	for (i = 0; i < hash_table->num_of_buckets; ++i)
	{
		DLListDestroy(*(table_iter + i));
	}

	free(hash_table->table);
	free(hash_table);
}

int HashInsert(hash_table_t *hash_table, void *data)
{
	size_t bucket_index = 0;
	dl_list_t *bucket = NULL;
	dll_iter_t new_data_iter = NULL;

	assert(hash_table);
	
    bucket_index = hash_table->hash_func(data);
	assert(bucket_index <= hash_table->num_of_buckets);

	bucket = *(hash_table->table + bucket_index);
	new_data_iter = DLListPushFront(bucket, data);
	if (DLListIsSameIterator(new_data_iter, DLListEnd(bucket)))
	{
		return 1;
	}

	return 0;
}

void HashRemove(hash_table_t *hash_table, const void *data)
{

	dl_list_t *bucket = NULL;
	dll_iter_t node_to_remove = NULL;
	
	assert(hash_table);

	bucket = GetBucketIMP(hash_table, data);
	node_to_remove = DLListFind(DLListBegin(bucket),
				 				DLListEnd(bucket),
		             			data,
					 			hash_table->match_func);
	if (!DLListIsSameIterator(node_to_remove, DLListEnd(bucket)))
	{
		DLListRemove(node_to_remove);
	}
}

void *HashFind(const hash_table_t *hash_table, const void *data)
{
	dl_list_t *bucket = NULL;
	dll_iter_t node_with_data = NULL;
	
	assert(hash_table);

	bucket = GetBucketIMP(hash_table, data);
	node_with_data = DLListFind(DLListBegin(bucket),
			     				DLListEnd(bucket),
	                			data,
								hash_table->match_func);
	if (DLListIsSameIterator(node_with_data, DLListEnd(bucket)))
	{
		return NULL;
	}
	
	return (DLListGetData(node_with_data));
}

static dl_list_t *GetBucketIMP(const hash_table_t *hash_table, const void *data)
{
	size_t bucket_index = hash_table->hash_func(data);
	assert(bucket_index <= hash_table->num_of_buckets);

	return *(hash_table->table + bucket_index);
}

int HashIsEmpty(const hash_table_t *hash_table)
{
	size_t i = 0;

	assert(hash_table);
	
	for (i = 0; i < hash_table->num_of_buckets; ++i)
	{
		if(!DLListIsEmpty(*(hash_table->table + i)))
		{
			return 0;
		}
	}

	return 1;
}

size_t HashSize(const hash_table_t *hash_table)
{
	size_t total_size = 0;
	size_t i = 0;

	assert(hash_table);

	for (i = 0; i < hash_table->num_of_buckets; ++i)
	{
		total_size += DLListSize(*(hash_table->table + i));
	}

	return total_size;
}

int HashForEach(hash_table_t *hash_table,
                action_func_t action_func,
                void *action_param)
{
	size_t i = 0;
	int return_value = 0;
	dl_list_t *bucket = NULL;

	assert(hash_table);
	assert(action_func);

	for (i = 0; i < hash_table->num_of_buckets; ++i)
	{
		bucket = *(hash_table->table + i);
		return_value = DLListForEach(DLListBegin(bucket),
									 DLListEnd(bucket),
								  	 action_param, 
									 action_func);
		if (0 != return_value)
		{
			return return_value;
		}
	}

	return 0;
}

double HashGetLoadFactor(const hash_table_t *hash_table)
{
	size_t num_of_elements = 0;
	size_t num_of_buckets = 0;
	
	assert(hash_table);

	num_of_elements = HashSize(hash_table);
	num_of_buckets  = hash_table->num_of_buckets;

	return (num_of_elements/num_of_buckets);
}

double HashGetStandardDeviation(const hash_table_t *hash_table)
{
	size_t i = 0;
	double res = 0;
	double load_factor = HashGetLoadFactor(hash_table);
	size_t num_of_buckets = hash_table->num_of_buckets;	

	assert(hash_table);	
	
	for (i = 0 ; i < hash_table -> num_of_buckets ; ++i)
	{
		res += pow((DLListSize(*(hash_table->table + i)) - load_factor), 2);
	}
	res /= num_of_buckets;
	
	return sqrt(res);
}
