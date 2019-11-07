/**********************************************************************
 *	This code is representing my test sequence of DS_SList RC3        *
 *	Lab: OL-79                                		                  *
 *	Author: Yonatan Vologdin 										  *
 *	Reviewer: Chen Rubin	    								      *
 *	Date: 07.11.19													  *
 **********************************************************************/
 
#include <stdio.h>	/*printf*/
#include <stdlib.h>	/*malloc*/
#include <assert.h>	/*assert*/

#include "slist.h"	/*singly linked list structure*/

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define RUN_TEST(test_name, result) \
(printf("%s %s\n", test_name, (1 == (result)) ? KGRN"\tPassed"KNRM : KRED"\tFailed"KNRM))

/*Auxiliary functions for Find & ForEach*/
static int LookForShort(const sl_node_t *node, void *param);
static int AddToInt(sl_node_t *node, void *param);

int SListTest()
{
	int head_data = 111;
	int node2_data = 6;
	int node1_data = 35;
	char node3_data = 'a';
	short node4_data = 20;
	float node5_data = 15.6;
	double node6_data = 1.78;
	
	int add_to_each = 1;
	for_each_ptr fe_func_ptr = AddToInt;
	find_ptr fp_func_ptr = LookForShort;

/* 						***Create Scenario***
 *---------------------------------------------------------------
 *  				   		node5  node6	  **add point to NULL
 *						  
 *			head  node2  node1  node3  node4  							
 */
	sl_node_t *head = SListCreateNode(&head_data, NULL);
	sl_node_t *node1 = SListCreateNode(&node1_data, NULL);
	sl_node_t *node2 = SListCreateNode(&node2_data, NULL);
	sl_node_t *node3 = SListCreateNode(&node3_data, NULL);	
	sl_node_t *node4 = SListCreateNode(&node4_data, NULL);
	sl_node_t *node5 = SListCreateNode(&node5_data, NULL);
	sl_node_t *node6 = SListCreateNode(&node6_data, NULL);

/*   					***Insert after Scenario***
 *---------------------------------------------------------------
 *			head -> node1 -> node3 -> node4 -> NULL 								
 */
	SListInsertAfter(node1, head);
	SListInsertAfter(node3, node1);
	SListInsertAfter(node4, node3);

/*   					***Insert Scenario***
 *---------------------------------------------------------------
 *			head -> node2 -> node1 -> node3 -> node4 -> NULL 								
 */
	/*inserts node2 before node1*/
	SListInsert(node2, node1);
	
	/*printf("List Count: %d\n",SListCount(head));*/
	if((5 != SListCount(head)) ||
	(6 != *(int*)(head->next)->data) ||
	(35 != *(int *)((head->next)->next)->data) ||
	('a' != *(char *)(((head->next)->next)->next)->data) ||
	(20 != *(short *)((((head->next)->next)->next)->next)->data))
	{
		printf("Failed Insert Scenario\n");
		
		return 0;
	}

/*   					***For each Scenario***
 *---------------------------------------------------------------
 *			head -> node2 -> node1 -> node3 -> node4 -> NULL 								
 */
 	/*printf("For each added 1: %d/4 nodes (not head)\n", SListForEach(head, &add_to_each, fe_func_ptr));*/
 	
	if((5 != SListCount(head)) ||
	(0 != SListForEach(head, &add_to_each, fe_func_ptr)) ||
	(7 != *(int*)(head->next)->data) ||
	(36 != *(int *)((head->next)->next)->data) ||
	('b' != *(char *)(((head->next)->next)->next)->data) ||
	(21 != *(short *)((((head->next)->next)->next)->next)->data))
	{
		printf("Failed For each Scenario\n");
		
		return 0;		
	}

/*   					***Find Scenario***
 *---------------------------------------------------------------
 *			head -> node2 -> node1 -> node3 -> node4 -> NULL 								
 */

	if (node4 != SListFind(head, &node4_data, fp_func_ptr))
	{
		printf("Failed Find Scenario\n");
		
		return 0;	
	}

/* 						***Loop Scenario***
 *---------------------------------------------------------------
 *  node5 points to node1  /<- node5 <- \	node4 points to node5			
 *						  / 		     \
 *	head -> node2 -> node1 -> node3 -> node4
 */

	/*creating loop*/
	node4->next = node5;
	node5->next = node1;

	/*printf("Has loop:%d\n", SListHasLoop(head));*/
	if (1 != SListHasLoop(head))
	{
		printf("Failed Loop Scenario\n");
		
		return 0;	
	}

/* 					***Intersection Scenario***
 *---------------------------------------------------------------
 *  node5 points to node1  /<- node5 <- node6		
 *						  /
 *	head -> node2 -> node1 -> node3 -> node4 -> NULL							
 */

	/*adjusting for scenario*/
	node4->next = NULL;
	node6->next = node5;

	if (node1 != SListFindIntersection(head, node6))
	{
		printf("Failed Intersection Scenario\n");
		
		return 0;
	}

/* 					***No intersection Scenario***
 *---------------------------------------------------------------
 *  node5 points to node1  NULL <- node5 <- node6		
 *						  
 *	head -> node2 -> node1 -> node3 -> node4 -> NULL 								
 */
 	node5->next = NULL;
	if ((NULL != SListFindIntersection(head, node6)) ||
	(NULL != SListFindIntersection(head, node1)))
	{
		printf("Failed No intersection Scenario\n");
		
		return 0;
	}

/* 					***Flip Scenario***
 *---------------------------------------------------------------		
 *	  NULL <- head <- node2 <- node1 <- node3 <- node4 								
 */
	/*adjust for scenario*/
 	SListFreeAll(node6);
 	
	head = SListFlip(head);
	node4->next = node3;
	if (5 != SListCount(head))
	{
		printf("Failed Flip Scenario\n");
		
		return 0;
	}

/*   					***Remove Scenario***
 *---------------------------------------------------------------
 *	before:		head -> node2 -> node1 -> node3 -> node4 -> NULL 								
 *	after:		head -> node2 ----------> node3 -> node4 -> NULL						
 */
 	/*adjust for scenario*/
	head = SListFlip(head);
 
	/*removes node1*/
	SListRemove((head->next)->next);
	free(node3); /*actually removes next node*/
	
	if((4 != SListCount(head)) ||
	(7 != *(int*)(head->next)->data) ||
	('b' != *(char *)((head->next)->next)->data) ||
	(21 != *(short *)((((head->next)->next)->next)->data)))
	{
		printf("Failed Remove Scenario\n");
		
		return 0;
	}
  	
/*   					***Remove After Scenario***
 *---------------------------------------------------------------
 *	before:	head -> node2 -> node3 -> node4 -> NULL						
 *	after:	head -> node2 ----------> node4 -> NULL							
 */
	/*removes node3*/
	SListRemoveAfter((head->next)->next);
	free(node4); /*actually removes next node*/
	
	if((3 != SListCount(head)) ||
	(7 != *(int *)(head->next)->data) ||
	('b' != *(char *)((head->next)->next)->data))
	{
		printf("Failed Remove After Scenario\n");
		
		return 0;
	}

/*   					***Free Scenario***
 *---------------------------------------------------------------
 *			all memory freed (from head including)							
 */
	/*frees from node1 including*/
	SListFreeAll(head);
	
	return 1;
}

int main()
{
	RUN_TEST("SinglyListTest:", SListTest());

	return 0;
}

static int LookForShort(const sl_node_t *node, void *param)
{
	assert(NULL != node);
	assert(NULL != param);

	if(*(short *)param == *(short *)node->data)
	{
		return 1;
	}
	
	return 0;
}

static int AddToInt(sl_node_t *node, void *param)
{
	assert(NULL != node);

	*(int *)node->data += *(int *)param;
	
	return 0;
}
