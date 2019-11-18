/**********************************************************************
 *	This code is representing my tests of DS_SRTList RC4        	  *
 *	Lab: OL-79                                		                  *
 *	Author: Yonatan Vologdin 										  *
 *	Reviewer:Gal Salemon    	  								      *
 *	Date: 18.11.19													  *
 **********************************************************************/
#include <stdio.h>	/*printf*/
#include <assert.h>	/*assert*/
#include <string.h>	/*strcmp*/
#include <stdlib.h>	/*malloc*/

#include "sortedlist.h"	/*sorted list data structure*/

#define	UNUSE(var) ((void)(var))

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"

#define RUN_SEQUENCE(test_name, test)       \
printf("\n******* %s *******\n\n",test_name);\
test								          \

#define RUN_TEST(test_name, result) \
(printf("%s %s\n", test_name, (1 == (result)) ? KGRN"\tPassed"KNRM : KRED"\tFailed"KNRM))

/*user's sort method function*/
static int IsBefore(const void *data1, const void *data2, void *param);
static int AddParamToData(void *data, void *param);
static int AddParamToSalary(void *data, void *param);
static int IsParamMatchData(const void *data, const void *param);
static int BySal(const void *data1, const void *data2, void *param);
static int IsParamMatchSal(const void *data, const void *param);

typedef struct user_db
{
	char *name;
	int salary;
}person_struct;

int SrtListCreateDestroyTest();
int SrtListInsert1GetDataTest();
int SrtListInsert5BeginNextGetDataTest();
int SrtListInsert5PrevEndGetDataTest();
int SrtListRemoveTest();
int SrtListPopFrontBackTest();
int SrtListMergeTest();
int SrtListFindTest();
int SrtListFindIfTest();
int SrtListForEachTest();
int SrtListFindStructTest();
int SrtListFindIfStructTest();
int SrtListForEachStructTest();
int SrtListMiscTest();

int main()
{
	RUN_SEQUENCE("SrtListCreateDestroyTest" ,SrtListCreateDestroyTest());
	RUN_SEQUENCE("SrtListInsert1GetDataTest" ,SrtListInsert1GetDataTest());
	RUN_SEQUENCE("SrtListInsert5BeginNextGetDataTest" ,SrtListInsert5BeginNextGetDataTest());
	RUN_SEQUENCE("SrtListInsert5PrevEndGetDataTest" ,SrtListInsert5PrevEndGetDataTest());
	RUN_SEQUENCE("SrtListRemoveTest" ,SrtListRemoveTest());
	RUN_SEQUENCE("SrtListPopFrontBackTest" ,SrtListPopFrontBackTest());
	RUN_SEQUENCE("SrtListMergeTest" ,SrtListMergeTest());
	RUN_SEQUENCE("SrtListFindTest" ,SrtListFindTest());
	RUN_SEQUENCE("SrtListFindIfTest" ,SrtListFindIfTest());
	RUN_SEQUENCE("SrtListForEachTest" ,SrtListForEachTest());
	RUN_SEQUENCE("SrtListFindStructTest" ,SrtListFindStructTest());
	RUN_SEQUENCE("SrtListFindIfStructTest" ,SrtListFindIfStructTest());
	RUN_SEQUENCE("SrtListForEachStructTest" ,SrtListForEachStructTest());
	RUN_SEQUENCE("SrtListMiscTest" ,SrtListMiscTest());

	return 0;
}

int SrtListCreateDestroyTest()
{	
	int param = 1;
	srt_list_t *srt_list = NULL;
	srt_list = SrtListCreate(&param, IsBefore);

	RUN_TEST("CreateTest:", NULL != srt_list);

	SrtListDestroy(srt_list);

	return 0;
}

int SrtListInsert1GetDataTest()
{
	int param = 1;
	int data1 = 5;

	srt_iter_t srt_new_node = {NULL};
	srt_list_t *srt_list = NULL;
	srt_list = SrtListCreate(&param, IsBefore);
	
	srt_new_node = SrtListInsert(&data1, srt_list);
	RUN_TEST("InsertAndGetDataTest:", 5 == *(int *)SrtListGetData(srt_new_node));
	
	SrtListDestroy(srt_list);

	return 0;
}

int SrtListInsert5BeginNextGetDataTest()
{
	int param = 1;

	int data1 = 1;
	int data2 = 5;
	int data3 = 3;
	int data4 = 2;
	int data5 = 4;
	srt_iter_t srt_iter = {NULL};
	srt_list_t *srt_list = NULL;

	srt_list = SrtListCreate(&param, IsBefore);

	SrtListInsert(&data1, srt_list);
	SrtListInsert(&data2, srt_list);
	SrtListInsert(&data3, srt_list);
	SrtListInsert(&data4, srt_list);
	SrtListInsert(&data5, srt_list);

	/*running forward*/	
	srt_iter = SrtListBegin(srt_list);
	RUN_TEST("InsertAndGetDataBeginTest:", 1 == *(int *)SrtListGetData(srt_iter));
	srt_iter = SrtListNext(srt_iter);
	RUN_TEST("InsertAndGetDataNextTest1:", 2 == *(int *)SrtListGetData(srt_iter));
	srt_iter = SrtListNext(srt_iter);
	RUN_TEST("InsertAndGetDataNextTest2:", 3 == *(int *)SrtListGetData(srt_iter));
	srt_iter = SrtListNext(srt_iter);
	RUN_TEST("InsertAndGetDataNextTest3:", 4 == *(int *)SrtListGetData(srt_iter));
	srt_iter = SrtListNext(srt_iter);
	RUN_TEST("InsertAndGetDataNextTest4:", 5 == *(int *)SrtListGetData(srt_iter));
	
	SrtListDestroy(srt_list);

	return 0;
}

int SrtListInsert5PrevEndGetDataTest()
{
	int param = 1;

	int data1 = 1;
	int data2 = 5;
	int data3 = 3;
	int data4 = 2;
	int data5 = 4;
	srt_iter_t srt_iter = {NULL};
	srt_list_t *srt_list = NULL;

	srt_list = SrtListCreate(&param, IsBefore);

	SrtListInsert(&data1, srt_list);
	SrtListInsert(&data2, srt_list);
	SrtListInsert(&data3, srt_list);
	SrtListInsert(&data4, srt_list);
	SrtListInsert(&data5, srt_list);

	/*running backwards*/	
	srt_iter = SrtListPrev(SrtListEnd(srt_list));
	RUN_TEST("InsertAndGetDataEndTest:", 5 == *(int *)SrtListGetData(srt_iter));
	srt_iter = SrtListPrev(srt_iter);
	RUN_TEST("InsertAndGetDataPrevTest1:", 4 == *(int *)SrtListGetData(srt_iter));
	srt_iter = SrtListPrev(srt_iter);
	RUN_TEST("InsertAndGetDataPrevTest2:", 3 == *(int *)SrtListGetData(srt_iter));
	srt_iter = SrtListPrev(srt_iter);
	RUN_TEST("InsertAndGetDataPrevTest3:", 2 == *(int *)SrtListGetData(srt_iter));
	srt_iter = SrtListPrev(srt_iter);
	RUN_TEST("InsertAndGetDataPrevTest4:", 1 == *(int *)SrtListGetData(srt_iter));
	SrtListDestroy(srt_list);

	return 0;
}

int SrtListRemoveTest()
{
	int param = 1;

	int data1 = 1;
	int data2 = 5;
	int data3 = 3;
	int data4 = 2;
	int data5 = 4;
	srt_iter_t srt_iter = {NULL};
	srt_list_t *srt_list = NULL;

	srt_list = SrtListCreate(&param, IsBefore);

	SrtListInsert(&data1, srt_list);
	SrtListInsert(&data2, srt_list);
	SrtListInsert(&data3, srt_list);
	SrtListInsert(&data4, srt_list);
	SrtListInsert(&data5, srt_list);

	srt_iter = SrtListBegin(srt_list);	/*at node1*/
	srt_iter = SrtListNext(srt_iter);	/*at node2*/
	srt_iter = SrtListRemove(srt_iter);	/*removed node 2, now at node3*/
	srt_iter = SrtListNext(srt_iter);	/*at node4*/
			   SrtListRemove(srt_iter);	/*removed node 4*/

	/*running forward*/	
	srt_iter = SrtListBegin(srt_list);
	RUN_TEST("RemoveTest1:", 1 == *(int *)SrtListGetData(srt_iter));
	srt_iter = SrtListNext(srt_iter);
	RUN_TEST("RemoveTest2:", 3 == *(int *)SrtListGetData(srt_iter));
	srt_iter = SrtListNext(srt_iter);
	RUN_TEST("RemoveTest3:", 5 == *(int *)SrtListGetData(srt_iter));

	/*running backwards*/	
	srt_iter = SrtListPrev(SrtListEnd(srt_list));
	RUN_TEST("RemoveTest4:", 5 == *(int *)SrtListGetData(srt_iter));
	srt_iter = SrtListPrev(srt_iter);
	RUN_TEST("RemoveTest5:", 3 == *(int *)SrtListGetData(srt_iter));
	srt_iter = SrtListPrev(srt_iter);
	RUN_TEST("RemoveTest6:", 1 == *(int *)SrtListGetData(srt_iter));

	SrtListDestroy(srt_list);

	return 0;
}

int SrtListPopFrontBackTest()
{
	int param = 1;

	int data1 = 1;
	int data2 = 5;
	int data3 = 3;
	int data4 = 2;
	int data5 = 4;
	srt_iter_t srt_iter = {NULL};
	srt_list_t *srt_list = NULL;

	srt_list = SrtListCreate(&param, IsBefore);

	SrtListInsert(&data1, srt_list);
	SrtListInsert(&data2, srt_list);
	SrtListInsert(&data3, srt_list);
	SrtListInsert(&data4, srt_list);
	SrtListInsert(&data5, srt_list);

	SrtListPopBack(srt_list);
	SrtListPopFront(srt_list);
	SrtListPopBack(srt_list);
	SrtListPopBack(srt_list);
	
	/*running forward*/	
	srt_iter = SrtListBegin(srt_list);
	RUN_TEST("InsertAndGetDataBeginTest:", 2 == *(int *)SrtListGetData(srt_iter));

	SrtListDestroy(srt_list);

	return 0;
}

int SrtListMergeTest()
{
	int param = 1;

	int src_data1 = 9;
	int src_data2 = 6;
	int src_data3 = 1;
	int src_data4 = 2;
	int src_data5 = 8;

	int dest_data1 = 5;
	int dest_data2 = 4;
	int dest_data3 = 7;
	int dest_data4 = 3;

	srt_iter_t srt_iter = {NULL};
	srt_list_t *srt_src_list = NULL;
	srt_list_t *srt_dest_list = NULL;

	srt_src_list = SrtListCreate(&param, IsBefore);
	srt_dest_list = SrtListCreate(&param, IsBefore);

	SrtListInsert(&src_data1, srt_src_list);
	SrtListInsert(&src_data2, srt_src_list);
	SrtListInsert(&src_data3, srt_src_list);
	SrtListInsert(&src_data4, srt_src_list);
	SrtListInsert(&src_data5, srt_src_list);

	SrtListInsert(&dest_data1, srt_dest_list);
	SrtListInsert(&dest_data2, srt_dest_list);
	SrtListInsert(&dest_data3, srt_dest_list);
	SrtListInsert(&dest_data4, srt_dest_list);

	SrtListMerge(srt_src_list, srt_dest_list);

	/*running forward*/
	srt_iter = SrtListBegin(srt_dest_list);
	RUN_TEST("MergeTest1:", 1 == *(int *)SrtListGetData(srt_iter));
	srt_iter = SrtListNext(srt_iter);
	RUN_TEST("MergeTest2:", 2 == *(int *)SrtListGetData(srt_iter));
	srt_iter = SrtListNext(srt_iter);
	RUN_TEST("MergeTest3:", 3 == *(int *)SrtListGetData(srt_iter));
	srt_iter = SrtListNext(srt_iter);
	RUN_TEST("MergeTest4:", 4 == *(int *)SrtListGetData(srt_iter));
	srt_iter = SrtListNext(srt_iter);
	RUN_TEST("MergeTest5:", 5 == *(int *)SrtListGetData(srt_iter));
	srt_iter = SrtListNext(srt_iter);
	RUN_TEST("MergeTest6:", 6 == *(int *)SrtListGetData(srt_iter));
	srt_iter = SrtListNext(srt_iter);
	RUN_TEST("MergeTest7:", 7 == *(int *)SrtListGetData(srt_iter));
	srt_iter = SrtListNext(srt_iter);
	RUN_TEST("MergeTest8:", 8 == *(int *)SrtListGetData(srt_iter));
	srt_iter = SrtListNext(srt_iter);
	RUN_TEST("MergeTest9:", 9 == *(int *)SrtListGetData(srt_iter));

	/*running forward*/
	srt_iter = SrtListPrev(SrtListEnd(srt_dest_list));
	RUN_TEST("MergeTest10:", 9 == *(int *)SrtListGetData(srt_iter));
	srt_iter = SrtListPrev(srt_iter);
	RUN_TEST("MergeTest11:", 8 == *(int *)SrtListGetData(srt_iter));
	srt_iter = SrtListPrev(srt_iter);
	RUN_TEST("MergeTest12:", 7 == *(int *)SrtListGetData(srt_iter));
	srt_iter = SrtListPrev(srt_iter);
	RUN_TEST("MergeTest13:", 6 == *(int *)SrtListGetData(srt_iter));
	srt_iter = SrtListPrev(srt_iter);
	RUN_TEST("MergeTest14:", 5 == *(int *)SrtListGetData(srt_iter));
	srt_iter = SrtListPrev(srt_iter);
	RUN_TEST("MergeTest15:", 4 == *(int *)SrtListGetData(srt_iter));
	srt_iter = SrtListPrev(srt_iter);
	RUN_TEST("MergeTest16:", 3 == *(int *)SrtListGetData(srt_iter));
	srt_iter = SrtListPrev(srt_iter);
	RUN_TEST("MergeTest17:", 2 == *(int *)SrtListGetData(srt_iter));
	srt_iter = SrtListPrev(srt_iter);
	RUN_TEST("MergeTest18:", 1 == *(int *)SrtListGetData(srt_iter));

	SrtListDestroy(srt_dest_list);

	return 0;
}

int SrtListFindTest()
{
	int create_param = 1;
	const int param1 = 2;
	const int param2 = 10;

	int data1 = 1;
	int data2 = 6;
	int data3 = 4;
	int data4 = 3;
	int data5 = 5;
	srt_iter_t match_iter1 = {NULL};
	srt_iter_t match_iter2 = {NULL};
	srt_iter_t node_iter = {NULL};
	srt_list_t *srt_list = NULL;

	srt_list = SrtListCreate(&create_param, IsBefore);

	SrtListInsert(&data1, srt_list);
	SrtListInsert(&data2, srt_list);
	SrtListInsert(&data3, srt_list);
	SrtListInsert(&data4, srt_list);
	SrtListInsert(&data5, srt_list);

	match_iter1 = SrtListFind(SrtListBegin(srt_list), SrtListEnd(srt_list), &param1, srt_list);
	match_iter2 = SrtListFind(SrtListBegin(srt_list), SrtListEnd(srt_list), &param2, srt_list);
	
	node_iter = SrtListBegin(srt_list);	
	node_iter = SrtListNext(node_iter);

	/*test if 2 exists at place 2 (should fail)*/
	RUN_TEST("FindTest1:", !SrtListIsSameIterator(node_iter, match_iter1));
	/*test if 10 doesnt exist and returns end*/
	RUN_TEST("FindTest2:",SrtListIsSameIterator
										(SrtListEnd(srt_list), match_iter2));

	SrtListDestroy(srt_list);

	return 0;
}

int SrtListFindIfTest()
{
 	int create_param = 1;
	const int param1 = 3;
	const int param2 = 10;

	int data1 = 1;
	int data2 = 5;
	int data3 = 3;
	int data4 = 2;
	int data5 = 4;
	srt_iter_t match_iter1 = {NULL};
	srt_iter_t match_iter2 = {NULL};
	srt_iter_t node3_iter = {NULL};
	srt_list_t *srt_list = NULL;
	is_match_func_t func_ptr = IsParamMatchData;
	srt_list = SrtListCreate(&create_param, IsBefore);

	SrtListInsert(&data1, srt_list);
	SrtListInsert(&data2, srt_list);
	SrtListInsert(&data3, srt_list);
	SrtListInsert(&data4, srt_list);
	SrtListInsert(&data5, srt_list);

	match_iter1 = SrtListFindIf(SrtListBegin(srt_list), SrtListEnd(srt_list), &param1, func_ptr);
	match_iter2 = SrtListFindIf(SrtListBegin(srt_list), SrtListEnd(srt_list), &param2, func_ptr);

	node3_iter = SrtListBegin(srt_list);	
	node3_iter = SrtListNext(node3_iter);	
	node3_iter = SrtListNext(node3_iter);	

	RUN_TEST("FindIfTest1:", SrtListIsSameIterator(node3_iter, match_iter1));
	RUN_TEST("FindIfTest2:",SrtListIsSameIterator
						(SrtListEnd(srt_list), match_iter2));

	SrtListDestroy(srt_list);

	return 0;
}

int SrtListForEachTest()
{
	int param = 10;

	int data1 = 1;
	int data2 = 5;
	int data3 = 3;
	int data4 = 2;
	int data5 = 4;
	srt_iter_t srt_iter = {NULL};
	srt_list_t *srt_list = NULL;
	action_func_t func_ptr = AddParamToData;
	srt_list = SrtListCreate(&param, IsBefore);

	SrtListInsert(&data1, srt_list);
	SrtListInsert(&data2, srt_list);
	SrtListInsert(&data3, srt_list);
	SrtListInsert(&data4, srt_list);
	SrtListInsert(&data5, srt_list);

	SrtListForEach(SrtListBegin(srt_list), SrtListEnd(srt_list), &param, func_ptr);

	srt_iter = SrtListBegin(srt_list);
	RUN_TEST("ForEachTest1:", 11 == *(int *)SrtListGetData(srt_iter));
	srt_iter = SrtListNext(srt_iter);
	RUN_TEST("ForEachTest2:", 12 == *(int *)SrtListGetData(srt_iter));
	srt_iter = SrtListNext(srt_iter);
	RUN_TEST("ForEachTest3:", 13 == *(int *)SrtListGetData(srt_iter));
	srt_iter = SrtListNext(srt_iter);
	RUN_TEST("ForEachTest4:", 14 == *(int *)SrtListGetData(srt_iter));
	srt_iter = SrtListNext(srt_iter);
	RUN_TEST("ForEachTest5:", 15 == *(int *)SrtListGetData(srt_iter));

	SrtListDestroy(srt_list);

	return 0;
}

int SrtListFindStructTest()
{
	/*int param = 1;*/
	const int sal6 = 6;
	/*const int sal7 = 7;*/
	const int sal20 = 20;

	char *name1 = "yosi";
	char *name2 = "avi";
	char *name3 = "babi";
	char *name4 = "cassandra";
	const int salary1 = 8;
	const int salary2 = 4;
	const int salary3 = 7;
	const int salary4 = 2;
	
	person_struct *person1 = NULL;
	person_struct *person2 = NULL;
	person_struct *person3 = NULL;
	person_struct *person4 = NULL;

	srt_list_t *srt_list = NULL;

	srt_iter_t match_iter1 = {NULL};
	srt_iter_t match_iter2 = {NULL};
	srt_iter_t node_iter = {NULL};

	person1 = (person_struct *)malloc(sizeof(person_struct));
	person1->name = name1;
	person1->salary = salary1;
	
	person2 = (person_struct *)malloc(sizeof(person_struct));
	person2->name = name2;
	person2->salary = salary2;
	
	person3 = (person_struct *)malloc(sizeof(person_struct));
	person3->name = name3;
	person3->salary = salary3;

	person4 = (person_struct *)malloc(sizeof(person_struct));
	person4->name = name4;
	person4->salary = salary4;
	
	srt_list = SrtListCreate(NULL, BySal);

	SrtListInsert(person1, srt_list);
	SrtListInsert(person2, srt_list);
	SrtListInsert(person3, srt_list);
	SrtListInsert(person4, srt_list);

	match_iter1 = SrtListFind(SrtListBegin(srt_list), SrtListEnd(srt_list), &sal6, srt_list);										
	match_iter2 = SrtListFind(SrtListBegin(srt_list), SrtListEnd(srt_list), &sal20, srt_list);
										
	node_iter = SrtListBegin(srt_list);	
	node_iter = SrtListNext(node_iter);	
	
	/*tries to find 6 at place 2, should fail*/
	RUN_TEST("FindStructTest1:",!SrtListIsSameIterator(node_iter, match_iter1));

	/*after trying to find 20 checks if equal to end*/
	RUN_TEST("FindStructTest2:",SrtListIsSameIterator(SrtListEnd(srt_list), match_iter2));

	free(person1);
	free(person2);
	free(person3);
	free(person4);
	SrtListDestroy(srt_list);

	return 0;
}

int SrtListFindIfStructTest()
{
	const int sal20 = 20;
	
	char *name1 = "yosi";
	char *name2 = "avi";
	char *name3 = "babi";
	char *name4 = "cassandra";
	int salary1 = 8;
	int salary2 = 4;
	int salary3 = 7;
	int salary4 = 2;
	
	person_struct *person1 = NULL;
	person_struct *person2 = NULL;
	person_struct *person3 = NULL;
	person_struct *person4 = NULL;

	srt_list_t *srt_list = NULL;

	srt_iter_t match_iter1 = {NULL};
	srt_iter_t match_iter2 = {NULL};
	srt_iter_t node3_iter = {NULL};

	is_match_func_t is_match = IsParamMatchSal;

	person1 = (person_struct *)malloc(sizeof(person_struct));
	person1->name = name1;
	person1->salary = salary1;
	
	person2 = (person_struct *)malloc(sizeof(person_struct));
	person2->name = name2;
	person2->salary = salary2;
	
	person3 = (person_struct *)malloc(sizeof(person_struct));
	person3->name = name3;
	person3->salary = salary3;

	person4 = (person_struct *)malloc(sizeof(person_struct));
	person4->name = name4;
	person4->salary = salary4;
	
	srt_list = SrtListCreate(NULL, BySal);

	SrtListInsert(person1, srt_list);
	SrtListInsert(person2, srt_list);
	SrtListInsert(person3, srt_list);
	SrtListInsert(person4, srt_list);

	match_iter1 = SrtListFindIf(SrtListBegin(srt_list), SrtListEnd(srt_list), &salary3, is_match);
	match_iter2 = SrtListFindIf(SrtListBegin(srt_list), SrtListEnd(srt_list), &sal20, is_match);

	node3_iter = SrtListBegin(srt_list);	
	node3_iter = SrtListNext(node3_iter);	
	node3_iter = SrtListNext(node3_iter);	

	RUN_TEST("FindIfStructTest1:", SrtListIsSameIterator(node3_iter, match_iter1));
	RUN_TEST("FindIfStructTest2:",SrtListIsSameIterator(SrtListEnd(srt_list), match_iter2));

	free(person1);
	free(person2);
	free(person3);
	free(person4);	
	SrtListDestroy(srt_list);
	
	return 0;
}

int SrtListForEachStructTest()
{
	int param = 1;
	int add_to_sal = 10;

	char *name1 = "yosi";
	char *name2 = "avi";
	char *name3 = "babi";
	char *name4 = "cassandra";
	int salary1 = 8;
	int salary2 = 4;
	int salary3 = 7;
	int salary4 = 2;
	
	person_struct *person1 = NULL;
	person_struct *person2 = NULL;
	person_struct *person3 = NULL;
	person_struct *person4 = NULL;

	srt_iter_t res_node = {NULL};
	srt_iter_t srt_iter = {NULL};
	srt_list_t *srt_list = NULL;

	action_func_t func_ptr = NULL;

	person1 = (person_struct *)malloc(sizeof(person_struct));
	person1->name = name1;
	person1->salary = salary1;
	
	person2 = (person_struct *)malloc(sizeof(person_struct));
	person2->name = name2;
	person2->salary = salary2;
	
	person3 = (person_struct *)malloc(sizeof(person_struct));
	person3->name = name3;
	person3->salary = salary3;

	person4 = (person_struct *)malloc(sizeof(person_struct));
	person4->name = name4;
	person4->salary = salary4;
	
	func_ptr = AddParamToSalary;
	srt_list = SrtListCreate(&param, BySal);

	res_node = SrtListInsert(person1, srt_list);
	SrtListInsert(person2, srt_list);
	SrtListInsert(person3, srt_list);
	SrtListInsert(person4, srt_list);

	param = (((person_struct *)SrtListGetData(res_node))->salary);

	SrtListForEach(SrtListBegin(srt_list), SrtListEnd(srt_list), &add_to_sal, func_ptr);

	srt_iter = SrtListBegin(srt_list);
	RUN_TEST("ForEachStructTest1:", 12 == (((person_struct *)SrtListGetData(srt_iter))->salary));
	srt_iter = SrtListNext(srt_iter);
	RUN_TEST("ForEachStructTest1:", 14 == (((person_struct *)SrtListGetData(srt_iter))->salary));
	srt_iter = SrtListNext(srt_iter);
	RUN_TEST("ForEachStructTest1:", 17 == (((person_struct *)SrtListGetData(srt_iter))->salary));
	srt_iter = SrtListNext(srt_iter);
	RUN_TEST("ForEachStructTest1:", 18 == (((person_struct *)SrtListGetData(srt_iter))->salary));

	free(person1);
	free(person2);
	free(person3);
	free(person4);
	SrtListDestroy(srt_list);

	return 0;
}

int SrtListMiscTest()
{
	int param = 1;

	int data1 = 1;
	int data2 = 5;
	int data3 = 3;
	int data4 = 2;
	int data5 = 4;
	srt_list_t *srt_list = NULL;

	srt_list = SrtListCreate(&param, IsBefore);
	RUN_TEST("IsSameIteratorTest1:", 1 == 		SrtListIsSameIterator(SrtListBegin(srt_list) ,SrtListEnd(srt_list)));

	RUN_TEST("IsEmptyTest1:", 1 == SrtListIsEmpty(srt_list));
	RUN_TEST("ListSizeTest1:", 0 == SrtListSize(srt_list));
	SrtListInsert(&data1, srt_list);
	RUN_TEST("ListSizeTest2:", 1 == SrtListSize(srt_list));
	SrtListInsert(&data2, srt_list);
	RUN_TEST("ListSizeTest3:", 2 == SrtListSize(srt_list));
	SrtListInsert(&data3, srt_list);
	RUN_TEST("ListSizeTest4:", 3 == SrtListSize(srt_list));
	SrtListInsert(&data4, srt_list);
	RUN_TEST("ListSizeTest5:", 4 == SrtListSize(srt_list));
	SrtListInsert(&data5, srt_list);
	RUN_TEST("ListSizeTest6:", 5 == SrtListSize(srt_list));
	RUN_TEST("IsEmptyTest2:", 0 == SrtListIsEmpty(srt_list));
	RUN_TEST("IsSameIteratorTest2:", 0 == SrtListIsSameIterator(SrtListNext(SrtListBegin(srt_list)) ,SrtListEnd(srt_list)));

	SrtListDestroy(srt_list);

	return 0;
}

static int IsParamMatchData(const void *data, const void *param)
{
	return (*(int *)data == *(int *)param);
}

static int IsParamMatchSal(const void *data, const void *param)
{
	person_struct *employee = NULL;
	int employee_sal = 0;

	assert(data);

	UNUSE(param);
	employee = (person_struct *)data;
	employee_sal = employee->salary;

	return (employee_sal == *(int *)param);
}

static int AddParamToData(void *data, void *param)
{
	assert(data);
	assert(param);

	*(int *)data += *(int *)param;
	
	return 0;
}

static int AddParamToSalary(void *data, void *param)
{
	person_struct *employee = NULL;
	int employee_sal = 0;

	assert(data);
	assert(param);

	employee = (person_struct *)data;
	employee_sal = employee->salary;
	employee_sal += *(int *)param;
	((person_struct *)data)->salary = employee_sal;
	
	return 0;
}

static int IsBefore(const void *data1, const void *data2, void *param)
{
	UNUSE(param);

	if (*(int *)data1 >= *(int *)data2)
	{		
		return 0;
	}

	return 1;
}

static int BySal(const void *data1, const void *data2, void *param)
{
	person_struct *employee1 = NULL;
	person_struct *employee2 = NULL;
	int employee_sal1 = 0;
	int employee_sal2 = 0;

	assert(data1);
	assert(data2);

	UNUSE(param);

	employee1 = (person_struct *)data1;
	employee2 = (person_struct *)data2;

	employee_sal1 = employee1->salary;
	employee_sal2 = employee2->salary;

	if (employee_sal1 >= employee_sal2)
	{		
		return 0;
	}

	return 1;
}
