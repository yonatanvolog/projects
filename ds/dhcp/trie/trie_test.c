/**********************************************************************
 *	This code is representing my tests of Trie         	              *
 *	Lab: OL-79                                		                  *
 *	Author: Yonatan Vologdin 										  *
 *	Date: 05.01.20						        					  *
 **********************************************************************/
#include <stdio.h>	    /*printf*/
#include <assert.h>	    /*assert*/
#include "trie.h"

#include "../../../yoni_utils.h" /*RUN_SEQUENCE, RUN_TEST*/

#define BYTE (8)

int TrieCreateDestroyTest();
int TrieInsert_Level2_Test();
int TrieInsert_Level3_Test();
int TrieDeallocateTest();
int TrieCountAllocTest();
int TrieCountFreeTest();
int TrieIsFullTest();

int main()
{
	RUN_SEQUENCE("TrieCreateDestroyTest", TrieCreateDestroyTest());
	RUN_SEQUENCE("TrieInsert_Level2_Test", TrieInsert_Level2_Test());
	RUN_SEQUENCE("TrieInsert_Level3_Test", TrieInsert_Level3_Test());
	RUN_SEQUENCE("TrieDeallocateTest", TrieDeallocateTest());
	RUN_SEQUENCE("TrieCountAllocTest", TrieCountAllocTest());
	RUN_SEQUENCE("TrieCountFreeTest", TrieCountFreeTest());
	RUN_SEQUENCE("TrieIsFullTest", TrieIsFullTest());
	
	return 0;
}

int TrieCreateDestroyTest()
{
    trie_t *trie = TrieCreate(2);
    assert(trie);
    
    printf("---Check Valgrind---\n");
    
    TrieDestroy(trie);
        
    return 0;
}

int TrieInsert_Level2_Test()
{
    trie_alloc_status_t status = 0;
    unsigned int requested_ip0 = 0; /*00*/
    unsigned int requested_ip1 = 1; /*01*/
    unsigned int requested_ip2 = 2; /*10*/
    unsigned int requested_ip3 = 3; /*11*/
    trie_t *trie = TrieCreate(2);
    assert(trie);
    
	status = TrieInsert(trie, requested_ip0);
	RUN_TEST("First Alloc 00\t", T_SUCCESS_ALLOCATED_REQUESTED == status);
    status = TrieInsert(trie, requested_ip0);
    RUN_TEST("Alloc of same address", T_REQUESTED_IP_OCCUPIED == status);

    status = TrieInsert(trie, requested_ip1);
	RUN_TEST("First Alloc 01\t", T_SUCCESS_ALLOCATED_REQUESTED == status);
    status = TrieInsert(trie, requested_ip1);
    RUN_TEST("Alloc of same address", T_REQUESTED_IP_OCCUPIED == status);

	status = TrieInsert(trie, requested_ip2);
	RUN_TEST("First Alloc 10\t", T_SUCCESS_ALLOCATED_REQUESTED == status);
    status = TrieInsert(trie, requested_ip2);
    RUN_TEST("Alloc of same address", T_REQUESTED_IP_OCCUPIED == status);

    status = TrieInsert(trie, requested_ip3);
	RUN_TEST("First Alloc 11\t", T_SUCCESS_ALLOCATED_REQUESTED == status);
    status = TrieInsert(trie, requested_ip3);
    RUN_TEST("Alloc of same address", T_REQUESTED_IP_OCCUPIED == status);
   
    TrieDestroy(trie);
   
    return 0;
}

int TrieInsert_Level3_Test()
{
    trie_alloc_status_t status = 0;
    unsigned int requested_ip0 = 0; /*000*/
    unsigned int requested_ip1 = 1; /*001*/
    unsigned int requested_ip2 = 2; /*010*/
    unsigned int requested_ip3 = 3; /*011*/
    unsigned int requested_ip4 = 4; /*100*/
    unsigned int requested_ip5 = 5; /*101*/
    unsigned int requested_ip6 = 6; /*110*/
    unsigned int requested_ip7 = 7; /*111*/
    trie_t *trie = TrieCreate(3);
    assert(trie);
    
	status = TrieInsert(trie, requested_ip0);
	RUN_TEST("First Alloc 000\t", T_SUCCESS_ALLOCATED_REQUESTED == status);
    status = TrieInsert(trie, requested_ip0);
    RUN_TEST("Alloc of same address", T_REQUESTED_IP_OCCUPIED == status);

    status = TrieInsert(trie, requested_ip1);
	RUN_TEST("First Alloc 001\t", T_SUCCESS_ALLOCATED_REQUESTED == status);
    status = TrieInsert(trie, requested_ip1);
    RUN_TEST("Alloc of same address", T_REQUESTED_IP_OCCUPIED == status);

	status = TrieInsert(trie, requested_ip2);
	RUN_TEST("First Alloc 010\t", T_SUCCESS_ALLOCATED_REQUESTED == status);
    status = TrieInsert(trie, requested_ip2);
    RUN_TEST("Alloc of same address", T_REQUESTED_IP_OCCUPIED == status);

    status = TrieInsert(trie, requested_ip3);
	RUN_TEST("First Alloc 011\t", T_SUCCESS_ALLOCATED_REQUESTED == status);
    status = TrieInsert(trie, requested_ip3);
    RUN_TEST("Alloc of same address", T_REQUESTED_IP_OCCUPIED == status);
    
    status = TrieInsert(trie, requested_ip4);
	RUN_TEST("First Alloc 100\t", T_SUCCESS_ALLOCATED_REQUESTED == status);
    status = TrieInsert(trie, requested_ip4);
    RUN_TEST("Alloc of same address", T_REQUESTED_IP_OCCUPIED == status);
    
    status = TrieInsert(trie, requested_ip5);
	RUN_TEST("First Alloc 101\t", T_SUCCESS_ALLOCATED_REQUESTED == status);
    status = TrieInsert(trie, requested_ip5);
    RUN_TEST("Alloc of same address", T_REQUESTED_IP_OCCUPIED == status);
    
    status = TrieInsert(trie, requested_ip6);
	RUN_TEST("First Alloc 110\t", T_SUCCESS_ALLOCATED_REQUESTED == status);
    status = TrieInsert(trie, requested_ip6);
    RUN_TEST("Alloc of same address", T_REQUESTED_IP_OCCUPIED == status);
    
    status = TrieInsert(trie, requested_ip7);
	RUN_TEST("First Alloc 111\t", T_SUCCESS_ALLOCATED_REQUESTED == status);
    status = TrieInsert(trie, requested_ip7);
    RUN_TEST("Alloc of same address", T_REQUESTED_IP_OCCUPIED == status);
   
    TrieDestroy(trie);
   
    return 0;
}

int TrieDeallocateTest()
{
    trie_alloc_status_t alloc_status = 0;
    trie_free_status_t free_status = 0;
    unsigned int ip0 = 0; /*00*/
    unsigned int ip1 = 1; /*01*/
    unsigned int ip2 = 2; /*10*/
    unsigned int ip3 = 3; /*11*/
    trie_t *trie = TrieCreate(2);
    assert(trie);
    
    free_status = TrieDeallocate(trie, ip0);
	RUN_TEST("Not Allocated Dealloc 00", T_IP_NOT_FOUND == free_status);
	
	TrieInsert(trie, ip0);
	free_status = TrieDeallocate(trie, ip0);
	RUN_TEST("Allocated Dealloc 00\t", T_SUCCESS == free_status);
	
    free_status = TrieDeallocate(trie, ip0);
	RUN_TEST("Dealloced Dealloc 00\t", T_DOUBLE_FREE == free_status);

    free_status = TrieDeallocate(trie, ip1);
	RUN_TEST("Not Allocated Dealloc 01", T_IP_NOT_FOUND == free_status);
    free_status = TrieDeallocate(trie, ip2);
	RUN_TEST("Not Allocated Dealloc 10", T_IP_NOT_FOUND == free_status);
	free_status = TrieDeallocate(trie, ip3);
	RUN_TEST("Not Allocated Dealloc 11", T_IP_NOT_FOUND == free_status);
	
	TrieInsert(trie, ip1);
	TrieInsert(trie, ip2);
	TrieInsert(trie, ip3);
	
	free_status = TrieDeallocate(trie, ip1);
	RUN_TEST("Allocated Dealloc 01\t", T_SUCCESS == free_status);
	free_status = TrieDeallocate(trie, ip2);
	RUN_TEST("Allocated Dealloc 10\t", T_SUCCESS == free_status);
	free_status = TrieDeallocate(trie, ip3);
	RUN_TEST("Allocated Dealloc 11\t", T_SUCCESS == free_status);
	
	free_status = TrieDeallocate(trie, ip1);
	RUN_TEST("Dealloced Dealloc 01\t", T_DOUBLE_FREE == free_status);
	free_status = TrieDeallocate(trie, ip2);
	RUN_TEST("Dealloced Dealloc 10\t", T_DOUBLE_FREE == free_status);
	free_status = TrieDeallocate(trie, ip3);
	RUN_TEST("Dealloced Dealloc 11\t", T_DOUBLE_FREE == free_status);
	
	alloc_status = TrieInsert(trie, ip0);
	RUN_TEST("Test Alloc 00\t", T_SUCCESS_ALLOCATED_REQUESTED == alloc_status);
	alloc_status = TrieInsert(trie, ip1);
	RUN_TEST("Test Alloc 01\t", T_SUCCESS_ALLOCATED_REQUESTED == alloc_status);
	alloc_status = TrieInsert(trie, ip2);
	RUN_TEST("Test Alloc 10\t", T_SUCCESS_ALLOCATED_REQUESTED == alloc_status);
	alloc_status = TrieInsert(trie, ip3);
	RUN_TEST("Test Alloc 11\t", T_SUCCESS_ALLOCATED_REQUESTED == alloc_status);
	
    TrieDestroy(trie);
   
    return 0;
}

int TrieCountAllocTest()
{
    unsigned int requested_ip0 = 0; /*00*/
    unsigned int requested_ip1 = 1; /*01*/
    unsigned int requested_ip2 = 2; /*10*/
    unsigned int requested_ip3 = 3; /*11*/
    trie_t *trie = TrieCreate(2);
    assert(trie);
    
    RUN_TEST("No Allocated\t", 0 == TrieCountAlloc(trie));
    
	TrieInsert(trie, requested_ip0);
	RUN_TEST("Allocated 1\t", 1 == TrieCountAlloc(trie));
	TrieInsert(trie, requested_ip1);
	RUN_TEST("Allocated 2\t", 2 == TrieCountAlloc(trie));
	TrieInsert(trie, requested_ip2);
	RUN_TEST("Allocated 3\t", 3 == TrieCountAlloc(trie));
	TrieInsert(trie, requested_ip3);
	RUN_TEST("Allocated 4\t", 4 == TrieCountAlloc(trie));
    
    TrieDestroy(trie);
   
    return 0;
}

int TrieCountFreeTest()
{
    unsigned int requested_ip0 = 0; /*00*/
    unsigned int requested_ip1 = 1; /*01*/
    unsigned int requested_ip2 = 2; /*10*/
    unsigned int requested_ip3 = 3; /*11*/
    trie_t *trie = TrieCreate(2);
    assert(trie);
    
    RUN_TEST("No Allocated\t", 4 == TrieCountFree(trie));
    
	TrieInsert(trie, requested_ip0);
	RUN_TEST("Allocated 1\t", 3 == TrieCountFree(trie));
	TrieInsert(trie, requested_ip1);
	RUN_TEST("Allocated 2\t", 2 == TrieCountFree(trie));
	TrieInsert(trie, requested_ip2);
	RUN_TEST("Allocated 3\t", 1 == TrieCountFree(trie));
	TrieInsert(trie, requested_ip3);
	RUN_TEST("Allocated 4\t", 0 == TrieCountFree(trie));

	TrieDeallocate(trie, requested_ip0);
    RUN_TEST("Deallocated\t", 1 == TrieCountFree(trie));
	TrieDeallocate(trie, requested_ip1);
    RUN_TEST("Deallocated\t", 2 == TrieCountFree(trie));
	TrieDeallocate(trie, requested_ip2);
    RUN_TEST("Deallocated\t", 3 == TrieCountFree(trie));
	TrieDeallocate(trie, requested_ip3);
    RUN_TEST("Deallocated\t", 4 == TrieCountFree(trie));
    
    TrieDestroy(trie);
   
    return 0;
}

int TrieIsFullTest()
{
    unsigned int requested_ip0 = 0; /*00*/
    unsigned int requested_ip1 = 1; /*01*/
    unsigned int requested_ip2 = 2; /*10*/
    unsigned int requested_ip3 = 3; /*11*/
    trie_t *trie = TrieCreate(2);
    assert(trie);
    
    RUN_TEST("No Allocated\t", 0 == TrieIsFull(trie));
    
	TrieInsert(trie, requested_ip0);
	RUN_TEST("Allocated 1\t", 0 == TrieIsFull(trie));
	TrieInsert(trie, requested_ip1);
	RUN_TEST("Allocated 2\t", 0 == TrieIsFull(trie));
	TrieInsert(trie, requested_ip2);
	RUN_TEST("Allocated 3\t", 0 == TrieIsFull(trie));
	TrieInsert(trie, requested_ip3);
	RUN_TEST("Allocated 4\t", 1 == TrieIsFull(trie));
    
    TrieDeallocate(trie, requested_ip3);
    RUN_TEST("Deallocated\t", 0 == TrieIsFull(trie));
    TrieInsert(trie, requested_ip3);
	RUN_TEST("Allocated again\t", 1 == TrieIsFull(trie));
    
    TrieDestroy(trie);
   
    return 0;
}


















