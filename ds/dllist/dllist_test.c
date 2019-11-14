/**********************************************************************
 *	This code is representing my tests of DS_DLList RC2               *
 *	Lab: OL-79                                		                  *
 *	Author: Yonatan Vologdin 										  *
 *	Reviewer: Dvir Breitbart 	  								      *
 *	Date: 13.11.19													  *
 **********************************************************************/
#include <stdio.h>	/*printf*/
#include <assert.h>	/*assert*/
#include <string.h>	/*strcmp*/

#include "dllist.h"	/*doubly linked list data structure*/

#define RESULT(condition) ((condition) ? 1 : 0)

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"

#define RUN_SEQUENCE(test_name, test)       \
printf("\n******* %s *******\n\n",test_name);\
test								          \

#define RUN_TEST(test_name, result) \
(printf("%s %s\n", test_name, (1 == (result)) ? KGRN"\tPassed"KNRM : KRED"\tFailed"KNRM))

struct dl_list
{
	dll_node_t *begin;
	dll_node_t *end;
};

struct node 
{
	void *data;
	struct node *prev;
	struct node *next;
};

static int AddToInt(dll_node_t *iter, void *param);
static int LookForNum(const dll_node_t *iter, const void *param);

int DLListCreateAndDestroyTest();
int DLListInsertTest();
int DLListPushTest();
int DLListRemoveTest();
int DLListPopTest();
int DLListForEachTest();
int DLListFindTest();
int DLListSpliceTest();
int DLListMiscTest();

int main()
{
	RUN_SEQUENCE("DLListCreateAndDestroyTest" ,DLListCreateAndDestroyTest());
	RUN_SEQUENCE("DLListInsertTest" ,DLListInsertTest());
	RUN_SEQUENCE("DLListPushTest" ,DLListPushTest());
	RUN_SEQUENCE("DLListRemoveTest" ,DLListRemoveTest());
	RUN_SEQUENCE("DLListPopTest" ,DLListPopTest());
	RUN_SEQUENCE("DLListForEachTest" ,DLListForEachTest());
	RUN_SEQUENCE("DLListFindTest" ,DLListFindTest());
	RUN_SEQUENCE("DLListSpliceTest" ,DLListSpliceTest());
	RUN_SEQUENCE("DLListMiscTest" ,DLListMiscTest());

	return 0;
}

int DLListCreateAndDestroyTest()
{
	dl_list_t *list = NULL;
	list = DLListCreate();
	RUN_TEST("CreateTest1:",  list->begin->data == ((void *)(0xDEADBEEF)));
	RUN_TEST("CreateTest2:", list->end->data == ((void *)(0xDEADBEEF)));
	DLListDestroy(list);

	return 0;
}

int DLListInsertTest()
{
	dl_list_t *list = NULL;
	dll_iter_t run_iter = NULL;

	list = DLListCreate();
	DLListInsert("node1_data", list->end, list);	
	DLListInsert("node2_data", list->end, list);	
	DLListInsert("node3_data", list->end, list);

	/*from begin till end*/
	run_iter = DLListBegin(list);
	RUN_TEST("InsertTest1:", !strcmp("node1_data", (char *)run_iter->data));

	run_iter = DLListNext(run_iter);
	RUN_TEST("InsertTest2:", !strcmp("node2_data", (char *)run_iter->data));

	run_iter = DLListNext(run_iter);
	RUN_TEST("InsertTest3:", !strcmp("node3_data", (char *)run_iter->data));

	/*from end till begin*/
	run_iter = DLListNext(run_iter);
	run_iter = DLListPrev(run_iter);
	RUN_TEST("InsertTest4:", !strcmp("node3_data", (char *)run_iter->data));

	run_iter = DLListPrev(run_iter);
	RUN_TEST("InsertTest5:", !strcmp("node2_data", (char *)run_iter->data));

	run_iter = DLListPrev(run_iter);
	RUN_TEST("InsertTest6:", !strcmp("node1_data", (char *)run_iter->data));

	DLListDestroy(list);

	return 0;
}

int DLListPushTest()
{
	dl_list_t *list = NULL;
	dll_iter_t run_iter = NULL;

	list = DLListCreate();
	
	/*end result: begin-> 1 2 3 4 5 6 ->end*/
	DLListPushFront(list, "node3_data");	/*id_1*/
	DLListPushBack(list, "node4_data");		/*id_2*/
	DLListPushFront(list, "node2_data");	/*id_3*/
	DLListPushFront(list, "node1_data");	/*id_4*/
	DLListPushBack(list, "node5_data");		/*id_5*/
	DLListPushBack(list, "node6_data");		/*id_6*/

	/*from begin till end*/
	run_iter = DLListBegin(list);
	RUN_TEST("PushTest1:", !strcmp("node1_data", (char *)run_iter->data));

	run_iter = DLListNext(run_iter);
	RUN_TEST("PushTest2:", !strcmp("node2_data", (char *)run_iter->data));

	run_iter = DLListNext(run_iter);
	RUN_TEST("PushTest3:", !strcmp("node3_data", (char *)run_iter->data));

	run_iter = DLListNext(run_iter);
	RUN_TEST("PushTest4:", !strcmp("node4_data", (char *)run_iter->data));

	run_iter = DLListNext(run_iter);
	RUN_TEST("PushTest5:", !strcmp("node5_data", (char *)run_iter->data));

	run_iter = DLListNext(run_iter);
	RUN_TEST("PushTest6:", !strcmp("node6_data", (char *)run_iter->data));

	/*from end till begin*/
	run_iter = DLListNext(run_iter);
	run_iter = DLListPrev(run_iter);
	RUN_TEST("PushTest7:", !strcmp("node6_data", (char *)run_iter->data));

	run_iter = DLListPrev(run_iter);
	RUN_TEST("PushTest8:", !strcmp("node5_data", (char *)run_iter->data));

	run_iter = DLListPrev(run_iter);
	RUN_TEST("PushTest9:", !strcmp("node4_data", (char *)run_iter->data));

	run_iter = DLListPrev(run_iter);
	RUN_TEST("PushTest10:", !strcmp("node3_data", (char *)run_iter->data));

	run_iter = DLListPrev(run_iter);
	RUN_TEST("PushTest11:", !strcmp("node2_data", (char *)run_iter->data));

	run_iter = DLListPrev(run_iter);
	RUN_TEST("PushTest12:", !strcmp("node1_data", (char *)run_iter->data));

	DLListDestroy(list);

	return 0;
}

int DLListRemoveTest()
{
	dl_list_t *list = NULL;
	dll_iter_t run_iter = NULL;

	dll_iter_t static_iter1 = NULL;
	dll_iter_t static_iter3 = NULL;
	dll_iter_t static_iter5 = NULL;

	list = DLListCreate();
	static_iter1 = DLListInsert("node1_data", list->end, list);	
				   DLListInsert("node2_data", list->end, list);	
	static_iter3 = DLListInsert("node3_data", list->end, list);
				   DLListInsert("node4_data", list->end, list);
	static_iter5 = DLListInsert("node5_data", list->end, list);

	run_iter = DLListBegin(list);
	run_iter = DLListNext(run_iter); 	/*run_iter at node2*/
	run_iter = DLListRemove(run_iter);	/*removes node2, now run_iter at node3*/
	RUN_TEST("RemoveTest1:", (DLListNext(static_iter1) == static_iter3));

	run_iter = DLListNext(run_iter); 	/*run_iter at node4*/
	run_iter = DLListRemove(run_iter);	/*removes node4, now run_iter at node5*/
	RUN_TEST("RemoveTest2:", (DLListNext(static_iter3) == static_iter5));

	run_iter = DLListPrev(run_iter);	/*run_iter at node3*/
	run_iter = DLListPrev(run_iter);	/*run_iter at node1*/
	run_iter = DLListRemove(run_iter);	/*removes node1, now run_iter at node3*/
	RUN_TEST("RemoveTest3:", (DLListNext(list->begin) == static_iter3));

	run_iter = DLListRemove(run_iter);	/*removes node3, now run_iter at node5*/
	RUN_TEST("RemoveTest4:", (DLListNext(list->begin) == static_iter5));

	run_iter = DLListRemove(run_iter);	/*removes node5, now run_iter at end*/
	RUN_TEST("RemoveTest5:", (DLListPrev(run_iter) == list->begin)
						   &&(DLListBegin(list) == run_iter));
	
	DLListDestroy(list);

	return 0;
}

int DLListPopTest()
{
	dl_list_t *list = NULL;

	list = DLListCreate();
	DLListInsert("node1_data", list->end, list);	
	DLListInsert("node2_data", list->end, list);	
	DLListInsert("node3_data", list->end, list);
	DLListInsert("node4_data", list->end, list);
	DLListInsert("node5_data", list->end, list);

	RUN_TEST("PopTest1:", !strcmp("node1_data", DLListPopFront(list)));
	RUN_TEST("PopTest2:", !strcmp("node5_data", DLListPopBack(list)));
	RUN_TEST("PopTest3:", !strcmp("node2_data", DLListPopFront(list)));
	RUN_TEST("PopTest4:", !strcmp("node3_data", DLListPopFront(list)));
	RUN_TEST("PopTest5:", !strcmp("node4_data", DLListPopBack(list)));

	DLListDestroy(list);	

	return 0;
}

int DLListForEachTest()
{
	dl_list_t *list = NULL;
	dll_iter_t run_iter = NULL;
	int node1_data = 1;
	int node2_data = 2;
	int node3_data = 3;
	int node4_data = 4;
	int node5_data = 5;
	int node6_data = 6;
	int to_add = 10;
	for_each_ptr fe_func_ptr = AddToInt;

	list = DLListCreate();
	DLListInsert(&node1_data, list->end, list);	
	DLListInsert(&node2_data, list->end, list);	
	DLListInsert(&node3_data, list->end, list);
	DLListInsert(&node4_data, list->end, list);
	DLListInsert(&node5_data, list->end, list);
	DLListInsert(&node6_data, list->end, list);

	/*starts from node3 including*/
	DLListForEach(DLListBegin(list)->next->next, DLListEnd(list), &to_add, fe_func_ptr);

	run_iter = DLListBegin(list);
	RUN_TEST("ForEachTest1:", (1 == *(int *)run_iter->data));
	run_iter = DLListNext(run_iter);
	RUN_TEST("ForEachTest2:", (2 == *(int *)run_iter->data));
	run_iter = DLListNext(run_iter);
	RUN_TEST("ForEachTest3:", (13 == *(int *)run_iter->data));
	run_iter = DLListNext(run_iter);
	RUN_TEST("ForEachTest4:", (14 == *(int *)run_iter->data));
	run_iter = DLListNext(run_iter);
	RUN_TEST("ForEachTest5:", (15 == *(int *)run_iter->data));
	run_iter = DLListNext(run_iter);
	RUN_TEST("ForEachTest6:", (16 == *(int *)run_iter->data));

	DLListDestroy(list);	

	return 0;
}

static int AddToInt(dll_node_t *iter, void *param)
{
	assert(NULL != iter);

	*(int *)iter->data += *(int *)param;
	
	return 0;
}

int DLListFindTest()
{
	dl_list_t *list = NULL;
	find_ptr fp_func_ptr = LookForNum;

	int node1_data = 1;
	int node2_data = 2;
	int node3_data = 3;
	int node4_data = 4;
	int node5_data = 5;
	int node6_data = 6;

	int num_to_find1 = 1;
	int num_to_find2 = 4;
	int num_to_find3 = 6;
	int num_to_find4 = 100;

	list = DLListCreate();
	DLListInsert(&node1_data, list->end, list);	
	DLListInsert(&node2_data, list->end, list);	
	DLListInsert(&node3_data, list->end, list);
	DLListInsert(&node4_data, list->end, list);
	DLListInsert(&node5_data, list->end, list);
	DLListInsert(&node6_data, list->end, list);

	/*starts from node1 including till node2 excluding*/
	RUN_TEST("FindTest1:", (1 == *(int *)DLListGetData
(DLListFind(DLListBegin(list), DLListBegin(list)->next, &num_to_find1, fp_func_ptr))));	

	/*starts from node2 including till end excluding*/
	RUN_TEST("FindTest2:", (4 == *(int *)DLListGetData
(DLListFind(DLListBegin(list)->next, DLListEnd(list), &num_to_find2, fp_func_ptr))));	

	/*starts from node1 including till end excluding*/
	RUN_TEST("FindTest3:", (6 == *(int *)DLListGetData
(DLListFind(DLListBegin(list), DLListEnd(list), &num_to_find3, fp_func_ptr))));

	/*starts from node1 including till end excluding*/
	RUN_TEST("FindTest4:", (DLListEnd(list) == DLListFind(DLListBegin(list), DLListEnd(list), &num_to_find4, fp_func_ptr)));	

	DLListDestroy(list);	

	return 0;
}

static int LookForNum(const dll_node_t *iter, const void *param)
{
	assert(NULL != iter);
	assert(NULL != param);

	if(*(int *)param == *(int *)iter->data)
	{
		return 1;
	}
	
	return 0;
}

int DLListSpliceTest()
{
	dl_list_t *src_list = NULL;
	dl_list_t *des_list = NULL;
	dll_iter_t run_iter = NULL;
	dll_iter_t des_iter = NULL;
	dll_iter_t src_begin_iter = NULL;
	dll_iter_t src_end_iter = NULL;

	int src_node1_data = 1;
	int src_node2_data = 2;
	int src_node3_data = 3;
	int src_node4_data = 4;
	int src_node5_data = 5;
	int src_node6_data = 6;
	
	int des_node1_data = 7;
	int des_node2_data = 8;
	int des_node3_data = 9;
	int des_node4_data = 10;
	
	src_list = DLListCreate();
					 DLListInsert(&src_node1_data, src_list->end, src_list);	
	src_begin_iter = DLListInsert(&src_node2_data, src_list->end, src_list);	
					 DLListInsert(&src_node3_data, src_list->end, src_list);
					 DLListInsert(&src_node4_data, src_list->end, src_list);
	src_end_iter   = DLListInsert(&src_node5_data, src_list->end, src_list);
					 DLListInsert(&src_node6_data, src_list->end, src_list);

	des_list = DLListCreate();
			   DLListInsert(&des_node1_data, des_list->end, des_list);	
			   DLListInsert(&des_node2_data, des_list->end, des_list);	
	des_iter = DLListInsert(&des_node3_data, des_list->end, des_list);
			   DLListInsert(&des_node4_data, des_list->end, des_list);

	DLListSplice(src_begin_iter, src_end_iter, des_iter);

	/*running through src list forward*/
	run_iter = src_list->begin;
	RUN_TEST("SrcRunningForwardTest1:", (run_iter == src_list->begin));
	run_iter = DLListNext(run_iter);
	RUN_TEST("SrcRunningForwardTest2:", (1 == *(int *)run_iter->data));
	run_iter = DLListNext(run_iter);
	RUN_TEST("SrcRunningForwardTest3:", (5 == *(int *)run_iter->data));
	run_iter = DLListNext(run_iter);
	RUN_TEST("SrcRunningForwardTest4:", (6 == *(int *)run_iter->data));
	run_iter = DLListNext(run_iter);
	RUN_TEST("SrcRunningForwardTest5:", (run_iter == src_list->end));

	/*running through src list backwards*/
	run_iter = DLListPrev(run_iter);
	RUN_TEST("SrcRunningBackwardsTest1:", (6 == *(int *)run_iter->data));
	run_iter = DLListPrev(run_iter);
	RUN_TEST("SrcRunningBackwardsTest2:", (5 == *(int *)run_iter->data));
	run_iter = DLListPrev(run_iter);
	RUN_TEST("SrcRunningBackwardsTest3:", (1 == *(int *)run_iter->data));
	run_iter = DLListPrev(run_iter);
	RUN_TEST("SrcRunningBackwardsTest4:", (run_iter == src_list->begin));

	/*running through dest list forward*/
	run_iter = des_list->begin;
	RUN_TEST("DestRunningForwardTest1:", (run_iter == des_list->begin));
	run_iter = DLListNext(run_iter);
	RUN_TEST("DestRunningForwardTest2:", (7 == *(int *)run_iter->data));
	run_iter = DLListNext(run_iter);
	RUN_TEST("DestRunningForwardTest3:", (8 == *(int *)run_iter->data));
	run_iter = DLListNext(run_iter);
	RUN_TEST("DestRunningForwardTest4:", (9 == *(int *)run_iter->data));
	run_iter = DLListNext(run_iter);
	RUN_TEST("DestRunningForwardTest5:", (2 == *(int *)run_iter->data));
	run_iter = DLListNext(run_iter);
	RUN_TEST("DestRunningForwardTest6:", (3 == *(int *)run_iter->data));
	run_iter = DLListNext(run_iter);
	RUN_TEST("DestRunningForwardTest7:", (4 == *(int *)run_iter->data));
	run_iter = DLListNext(run_iter);
	RUN_TEST("DestRunningForwardTest8:", (10 == *(int *)run_iter->data));
	run_iter = DLListNext(run_iter);
	RUN_TEST("DestRunningForwardTest9:", (run_iter == des_list->end));

	/*running through dest list backwards*/
	run_iter = DLListPrev(run_iter);
	RUN_TEST("DestRunningBackwardsTest1:", (10 == *(int *)run_iter->data));
	run_iter = DLListPrev(run_iter);
	RUN_TEST("DestRunningBackwardsTest2:", (4 == *(int *)run_iter->data));
	run_iter = DLListPrev(run_iter);
	RUN_TEST("DestRunningBackwardsTest3:", (3 == *(int *)run_iter->data));
	run_iter = DLListPrev(run_iter);
	RUN_TEST("DestRunningBackwardsTest4:", (2 == *(int *)run_iter->data));
	run_iter = DLListPrev(run_iter);
	RUN_TEST("DestRunningBackwardsTest5:", (9 == *(int *)run_iter->data));
	run_iter = DLListPrev(run_iter);
	RUN_TEST("DestRunningBackwardsTest6:", (8 == *(int *)run_iter->data));
	run_iter = DLListPrev(run_iter);
	RUN_TEST("DestRunningBackwardsTest7:", (7 == *(int *)run_iter->data));
	run_iter = DLListPrev(run_iter);
	RUN_TEST("DestRunningBackwardsTest8:", (run_iter == des_list->begin));

	DLListDestroy(src_list);	
	DLListDestroy(des_list);	

	return 0;
}

int DLListMiscTest()
{	
	dl_list_t *list = NULL;
	dll_iter_t static_iter1 = NULL;
	dll_iter_t static_iter2 = NULL;

	list = DLListCreate();

	RUN_TEST("IsEmptyTest1:", (1 == DLListIsEmpty(list)));

	RUN_TEST("SizeTest1:", (0 == DLListSize(list)));
	static_iter1 = DLListInsert("node1_data", list->end, list);
	RUN_TEST("SizeTest2:", (1 == DLListSize(list)));
	static_iter2 = DLListInsert("node1_data", list->end, list);
	RUN_TEST("SizeTest3:", (2 == DLListSize(list)));

	RUN_TEST("IsEmptyTest2:", (0 == DLListIsEmpty(list)));

	RUN_TEST("BeginTest:", DLListBegin(list) == list->begin->next);
	RUN_TEST("PrevTest:", DLListPrev(static_iter1) == list->begin);
	RUN_TEST("GetDataTest:", DLListGetData(static_iter2) == static_iter2->data);
	RUN_TEST("NextTest:", DLListNext(static_iter2) == list->end);
	RUN_TEST("EndTest:", DLListEnd(list) == list->end);

	RUN_TEST("IsSameIteratorTest1:", DLListIsSameIterator(static_iter1->next, static_iter2));
	RUN_TEST("IsSameIteratorTest2:", DLListIsSameIterator(static_iter1, static_iter2->prev));

	DLListDestroy(list);	

	return 0;
}
