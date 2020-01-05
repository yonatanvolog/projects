/**********************************************************************
 *	This code is representing my tests of Hashtable 	              *
 *	Lab: OL-79                                		                  *
 *	Author: Yonatan Vologdin 										  *
 *	Reviewer: Dvir Breitbart 	 	  						     	  *
 *	Date: 28.12.19													  *
 **********************************************************************/

#include <stdio.h>	/*printf*/
#include <stdlib.h>	/*malloc ,free*/
#include <assert.h>	/*assert*/
#include <math.h>	/*sqrt*/

#include "hashtable.h"
#include "../../yoni_utils.h"

/*Uncomment the line below in order to activate white-box tests*/
#define WHITE_BOX_TEST

#ifdef WHITE_BOX_TEST
#include "dllist.h"

struct hash
{
 hash_func_t hash_func;
 is_match_t match_func;
 size_t num_of_buckets;
 dl_list_t **table;
};
#endif

int HashCreateTest();
int HashInsertTest();
int HashRemoveTest();
int HashFindTest();
int HashIsEmptyTest();
int HashSizeTest();
int HashForEachTest();
int HashGetLoadFactorTest();
int HashGetStandardDeviationTest();

size_t HashFunc(const void *key);
int IsMatch(const void *current_data, const void *data_to_compare);
int IsMatchDllist(const void *data, const void *param);
int CounterFunc(void *data, void *action_param);

int main()
{
	RUN_SEQUENCE("HashCreateTest", HashCreateTest());
	RUN_SEQUENCE("HashInsertTest", HashInsertTest());
	RUN_SEQUENCE("HashRemoveTest", HashRemoveTest());
	RUN_SEQUENCE("HashFindTest", HashFindTest());
	RUN_SEQUENCE("HashIsEmptyTest", HashIsEmptyTest());
	RUN_SEQUENCE("HashSizeTest", HashSizeTest());
	RUN_SEQUENCE("HashForEachTest", HashForEachTest());
	RUN_SEQUENCE("HashGetLoadFactorTest", HashGetLoadFactorTest());
	RUN_SEQUENCE("HashGetStandardDeviationTest", HashGetStandardDeviationTest());

	return 0;
}

int HashCreateTest()
{
	hash_table_t *hash_table = HashCreate(10 ,HashFunc, IsMatch);
	if (NULL == hash_table)
	{
	    return 1;
	}

	HashDestroy(hash_table);

	return 0;
}

int HashInsertTest()
{
	int insert1 = 1;
	int insert2 = 2;
	int insert3 = 3;
	int insert4 = 4;
	int insert5 = 5;

	int insert11 = 11;
	int insert12 = 12;
	int insert13 = 13;
	int insert14 = 14;
	int insert15 = 15;

#ifdef WHITE_BOX_TEST
	dl_list_t **bucket = NULL;
	dll_iter_t found_iter = NULL;
	void *found_data = NULL;
#endif

	hash_table_t *hash_table = HashCreate(6 ,HashFunc, IsMatch);
	if (NULL == hash_table)
	{
	    return 1;
	}
	
	RUN_TEST("insert1 successful", 0 == HashInsert(hash_table, &insert1));
	RUN_TEST("insert2 successful", 0 == HashInsert(hash_table, &insert2));
	RUN_TEST("insert3 successful", 0 == HashInsert(hash_table, &insert3));
	RUN_TEST("insert4 successful", 0 == HashInsert(hash_table, &insert4));
	RUN_TEST("insert5 successful", 0 == HashInsert(hash_table, &insert5));

	RUN_TEST("insert11 successful", 0 == HashInsert(hash_table, &insert11));
	RUN_TEST("insert12 successful", 0 == HashInsert(hash_table, &insert12));
	RUN_TEST("insert13 successful", 0 == HashInsert(hash_table, &insert13));
	RUN_TEST("insert14 successful", 0 == HashInsert(hash_table, &insert14));
	RUN_TEST("insert15 successful", 0 == HashInsert(hash_table, &insert15));
	
#ifdef WHITE_BOX_TEST
	bucket = hash_table->table + 0;
	found_iter = DLListFind(DLListBegin(*bucket), DLListEnd(*bucket), &insert1, IsMatchDllist);
	RUN_TEST("wb find empty bucket", 1 == DLListIsSameIterator(DLListEnd(*bucket), found_iter));
	
	bucket = hash_table->table + 0;
	RUN_TEST("wb is bucket size", 0 == DLListSize(*bucket));
	

	bucket = hash_table->table + 1;
	found_iter = DLListFind(DLListBegin(*bucket), DLListEnd(*bucket), &insert1, IsMatchDllist);
	found_data = DLListGetData(found_iter);
	RUN_TEST("wb find insert1", &insert1 == found_data);

	bucket = hash_table->table + 1;
	found_iter = DLListFind(DLListBegin(*bucket), DLListEnd(*bucket), &insert11, IsMatchDllist);
	found_data = DLListGetData(found_iter);
	RUN_TEST("wb find insert11", &insert11 == found_data);

	bucket = hash_table->table + 1;
	RUN_TEST("wb is bucket size", 2 == DLListSize(*bucket));


	bucket = hash_table->table + 2;
	found_iter = DLListFind(DLListBegin(*bucket), DLListEnd(*bucket), &insert2, IsMatchDllist);
	found_data = DLListGetData(found_iter);
	RUN_TEST("wb find insert2", &insert2 == found_data);

	bucket = hash_table->table + 2;
	found_iter = DLListFind(DLListBegin(*bucket), DLListEnd(*bucket), &insert12, IsMatchDllist);
	found_data = DLListGetData(found_iter);
	RUN_TEST("wb find insert12", &insert12 == found_data);

	bucket = hash_table->table + 2;
	RUN_TEST("wb is bucket size", 2 == DLListSize(*bucket));


	bucket = hash_table->table + 3;
	found_iter = DLListFind(DLListBegin(*bucket), DLListEnd(*bucket), &insert3, IsMatchDllist);
	found_data = DLListGetData(found_iter);
	RUN_TEST("wb find insert3", &insert3 == found_data);

	bucket = hash_table->table + 3;
	found_iter = DLListFind(DLListBegin(*bucket), DLListEnd(*bucket), &insert13, IsMatchDllist);
	found_data = DLListGetData(found_iter);
	RUN_TEST("wb find insert13", &insert13 == found_data);

	bucket = hash_table->table + 3;
	RUN_TEST("wb is bucket size", 2 == DLListSize(*bucket));


	bucket = hash_table->table + 4;
	found_iter = DLListFind(DLListBegin(*bucket), DLListEnd(*bucket), &insert4, IsMatchDllist);
	found_data = DLListGetData(found_iter);
	RUN_TEST("wb find insert4", &insert4 == found_data);

	bucket = hash_table->table + 4;
	found_iter = DLListFind(DLListBegin(*bucket), DLListEnd(*bucket), &insert14, IsMatchDllist);
	found_data = DLListGetData(found_iter);
	RUN_TEST("wb find insert14", &insert14 == found_data);

	bucket = hash_table->table + 4;
	RUN_TEST("wb is bucket size", 2 == DLListSize(*bucket));


	bucket = hash_table->table + 5;
	found_iter = DLListFind(DLListBegin(*bucket), DLListEnd(*bucket), &insert5, IsMatchDllist);
	found_data = DLListGetData(found_iter);
	RUN_TEST("wb find insert5", &insert5 == found_data);

	bucket = hash_table->table + 5;
	found_iter = DLListFind(DLListBegin(*bucket), DLListEnd(*bucket), &insert15, IsMatchDllist);
	found_data = DLListGetData(found_iter);
	RUN_TEST("wb find insert15", &insert15 == found_data);

	bucket = hash_table->table + 5;
	RUN_TEST("wb is bucket size", 2 == DLListSize(*bucket));

#endif

	HashDestroy(hash_table);

	return 0;

}

int HashRemoveTest()
{
	int insert1 = 1;
	int insert2 = 2;
	int insert3 = 3;
	int insert4 = 4;
	int insert5 = 5;

	int insert11 = 11;
	int insert12 = 12;
	int insert13 = 13;
	int insert14 = 14;
	int insert15 = 15;

#ifdef WHITE_BOX_TEST
	dl_list_t **bucket = NULL;
	dll_iter_t found_iter = NULL;
#endif

	hash_table_t *hash_table = HashCreate(6 ,HashFunc, IsMatch);
	if (NULL == hash_table)
	{
	    return 1;
	}
	
	HashInsert(hash_table, &insert1);
	HashInsert(hash_table, &insert2);
	HashInsert(hash_table, &insert3);
	HashInsert(hash_table, &insert4);
	HashInsert(hash_table, &insert5);

	HashInsert(hash_table, &insert11);
	HashInsert(hash_table, &insert12);
	HashInsert(hash_table, &insert13);
	HashInsert(hash_table, &insert14);
	HashInsert(hash_table, &insert15);

	HashRemove(hash_table, &insert1);
#ifdef WHITE_BOX_TEST
	bucket = hash_table->table + 1;
	found_iter = DLListFind(DLListBegin(*bucket), DLListEnd(*bucket), &insert1, IsMatchDllist);
	RUN_TEST("wb find removed insert 1", 1 == DLListIsSameIterator(DLListEnd(*bucket), found_iter));
	
	bucket = hash_table->table + 1;
	RUN_TEST("wb is bucket size\t", 1 == DLListSize(*bucket));
#endif

	HashRemove(hash_table, &insert11);
#ifdef WHITE_BOX_TEST
	bucket = hash_table->table + 1;
	found_iter = DLListFind(DLListBegin(*bucket), DLListEnd(*bucket), &insert11, IsMatchDllist);
	RUN_TEST("wb find removed insert 11", 1 == DLListIsSameIterator(DLListEnd(*bucket), found_iter));
	
	bucket = hash_table->table + 1;
	RUN_TEST("wb is bucket size\t", 0 == DLListSize(*bucket));
#endif

	HashDestroy(hash_table);

	return 0;
}

int HashFindTest()
{
	int insert1 = 1;
	int insert2 = 2;
	int insert3 = 3;
	int insert4 = 4;
	int insert5 = 5;

	int insert11 = 11;
	int insert12 = 12;
	int insert13 = 13;
	int insert14 = 14;
	int insert15 = 15;

	hash_table_t *hash_table = HashCreate(6 ,HashFunc, IsMatch);
	if (NULL == hash_table)
	{
	    return 1;
	}
	
	HashInsert(hash_table, &insert1);
	HashInsert(hash_table, &insert2);
	HashInsert(hash_table, &insert3);
	HashInsert(hash_table, &insert4);
	HashInsert(hash_table, &insert5);

	HashInsert(hash_table, &insert11);
	HashInsert(hash_table, &insert12);
	HashInsert(hash_table, &insert13);
	HashInsert(hash_table, &insert14);
	HashInsert(hash_table, &insert15);

	RUN_TEST("found insert1", &insert1 == HashFind(hash_table, &insert1));
	RUN_TEST("found insert2", &insert2 == HashFind(hash_table, &insert2));
	RUN_TEST("found insert3", &insert3 == HashFind(hash_table, &insert3));
	RUN_TEST("found insert4", &insert4 == HashFind(hash_table, &insert4));
	RUN_TEST("found insert5", &insert5 == HashFind(hash_table, &insert5));

	RUN_TEST("found insert11", &insert11 == HashFind(hash_table, &insert11));
	RUN_TEST("found insert12", &insert12 == HashFind(hash_table, &insert12));
	RUN_TEST("found insert13", &insert13 == HashFind(hash_table, &insert13));
	RUN_TEST("found insert14", &insert14 == HashFind(hash_table, &insert14));
	RUN_TEST("found insert15", &insert15 == HashFind(hash_table, &insert15));

	HashDestroy(hash_table);

	return 0;
}

int HashIsEmptyTest()
{
	int insert1 = 1;
	int insert2 = 2;
	int insert3 = 3;
	int insert4 = 4;
	int insert5 = 5;

	hash_table_t *hash_table = HashCreate(6 ,HashFunc, IsMatch);
	if (NULL == hash_table)
	{
	    return 1;
	}
	
	RUN_TEST("before inserts", 1 == HashIsEmpty(hash_table));

	HashInsert(hash_table, &insert1);
	HashInsert(hash_table, &insert2);
	HashInsert(hash_table, &insert3);
	HashInsert(hash_table, &insert4);
	HashInsert(hash_table, &insert5);

	RUN_TEST("after inserts", 0 == HashIsEmpty(hash_table));

	HashRemove(hash_table, &insert1);
	HashRemove(hash_table, &insert2);
	HashRemove(hash_table, &insert3);
	HashRemove(hash_table, &insert4);
	HashRemove(hash_table, &insert5);

	RUN_TEST("after removes", 1 == HashIsEmpty(hash_table));

	HashDestroy(hash_table);

	return 0;
}

int HashSizeTest()
{
	int insert2 = 2;
	int insert4 = 4;
	int insert5 = 5;

	int insert11 = 11;
	int insert12 = 12;
	int insert14 = 14;
	int insert15 = 15;

	hash_table_t *hash_table = HashCreate(6 ,HashFunc, IsMatch);
	if (NULL == hash_table)
	{
	    return 1;
	}

	RUN_TEST("before inserts", 0 == HashSize(hash_table));	

	HashInsert(hash_table, &insert2);
	HashInsert(hash_table, &insert4);
	HashInsert(hash_table, &insert5);

	RUN_TEST("half inserts", 3 == HashSize(hash_table));	

	HashInsert(hash_table, &insert11);
	HashInsert(hash_table, &insert12);
	HashInsert(hash_table, &insert14);
	HashInsert(hash_table, &insert15);

	RUN_TEST("after inserts", 7 == HashSize(hash_table));	

	HashDestroy(hash_table);

	return 0;
}

int HashForEachTest()
{
	int insert1 = 1;
	int insert2 = 2;
	int insert3 = 3;
	int insert4 = 4;
	int insert5 = 5;

	int insert11 = 11;
	int insert12 = 12;
	int insert13 = 13;
	int insert14 = 14;
	int insert15 = 15;

	size_t counter = 0;

	hash_table_t *hash_table = HashCreate(6 ,HashFunc, IsMatch);
	if (NULL == hash_table)
	{
	    return 1;
	}
	
	HashInsert(hash_table, &insert1);
	HashInsert(hash_table, &insert2);
	HashInsert(hash_table, &insert3);
	HashInsert(hash_table, &insert4);
	HashInsert(hash_table, &insert5);

	HashInsert(hash_table, &insert11);
	HashInsert(hash_table, &insert12);
	HashInsert(hash_table, &insert13);
	HashInsert(hash_table, &insert14);
	HashInsert(hash_table, &insert15);

	HashForEach(hash_table, CounterFunc, &counter);
	
	RUN_TEST("counter", 10 == counter);

	HashDestroy(hash_table);

	return 0;
}

int HashGetLoadFactorTest()
{
	int insert1 = 1;
	int insert2 = 2;
	int insert3 = 3;
	int insert4 = 4;
	int insert5 = 5;

	double res = 0;

	hash_table_t *hash_table = HashCreate(6 ,HashFunc, IsMatch);
	if (NULL == hash_table)
	{
	    return 1;
	}
	
	HashInsert(hash_table, &insert1);
	HashInsert(hash_table, &insert2);
	HashInsert(hash_table, &insert3);
	HashInsert(hash_table, &insert4);
	HashInsert(hash_table, &insert5);
	
	res = HashGetLoadFactor(hash_table);
	RUN_TEST("load factor", (5/6) == res);

	HashDestroy(hash_table);

	return 0;
}

int HashGetStandardDeviationTest()
{
	int insert1 = 1;
	int insert2 = 2;
	int insert3 = 3;
	int insert4 = 4;
	int insert5 = 5;

	double res = 0;

	hash_table_t *hash_table = HashCreate(6 ,HashFunc, IsMatch);
	if (NULL == hash_table)
	{
	    return 1;
	}
	
	HashInsert(hash_table, &insert1);
	HashInsert(hash_table, &insert2);
	HashInsert(hash_table, &insert3);
	HashInsert(hash_table, &insert4);
	HashInsert(hash_table, &insert5);
	
	res = HashGetStandardDeviation(hash_table);
	RUN_TEST("stantard dev", 9.1287092917527690e-01 == res);

	HashDestroy(hash_table);

	return 0;
}




int CounterFunc(void *data, void *action_param)
{
	UNUSE(data);
	*(size_t *)action_param += 1;

	return 0;
}

int IsMatchDllist(const void *data, const void *param)
{
	return (*(int *)data == *(int *)param);
}

size_t HashFunc(const void *key)
{
    size_t index = *(int *)key % 10;
    
    return index;
}

int IsMatch(const void *current_data, const void *data_to_compare)
{
    return (*(int *)current_data == *(int *)data_to_compare);
}
