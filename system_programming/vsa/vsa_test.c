/**********************************************************************
 *	This code is representing my tests of VSA RC2			       	  *
 *	Lab: OL-79                                		                  *
 *	Author: Yonatan Vologdin 										  *
 *	Reviewer: Tamir Dayan        		  							  *
 *	Date: 03.12.19													  *
 **********************************************************************/
#include <stdio.h>			/*printf*/
#include <stdlib.h>			/*malloc*/

#include "vsa.h"			/*vsa*/
#include "../../yoni_utils.h"

#define HEADER_SIZE	8
#define MAGIC_NUM 	12345

int VSAInitTest();
int VSAInitTestDEBUG();
int VSAAllocTest();
int VSAAllocTestDEBUG();
int VSAFreeTest();
int VSAFreeTestDEBUG();
int VSADefragTest();
int VSADefragTestDEBUG();
int VSALargestChunkTest();
int VSALargestChunkTestDEBUG();

int main()
{
	#ifdef NDEBUG
	RUN_SEQUENCE("VSAInitTest", VSAInitTest());
	RUN_SEQUENCE("VSAAllocTest", VSAAllocTest());
	RUN_SEQUENCE("VSAFreeTest", VSAFreeTest());
	RUN_SEQUENCE("VSADefragTest", VSADefragTest());
	RUN_SEQUENCE("VSALargestChunkTest", VSALargestChunkTest());
	#endif

	#ifndef NDEBUG
	RUN_SEQUENCE("VSAInitTestDEBUG", VSAInitTestDEBUG());
	RUN_SEQUENCE("VSAAllocTestDEBUG", VSAAllocTestDEBUG());
	RUN_SEQUENCE("VSAFreeTestDEBUG", VSAFreeTestDEBUG());
	RUN_SEQUENCE("VSADefragTestDEBUG", VSADefragTestDEBUG());
	RUN_SEQUENCE("VSALargestChunkTestDEBUG", VSALargestChunkTestDEBUG());
	#endif

	return 0;
}

int VSAInitTest()
{
	size_t pool_size = 100;
	char *runner = NULL;

	char *vsa = (char *)malloc(pool_size);
	vsa = (char *)VSAInit(vsa, pool_size);

	runner = vsa;

	RUN_TEST("BeginHeaderTest", -80 == *(ssize_t *)runner);
	runner += (HEADER_SIZE + 80);
	RUN_TEST("EndHeaderTest\t", 0 == *(ssize_t *)runner);

	free(vsa);

	return 0;
}

int VSAInitTestDEBUG()
{
	size_t pool_size = 100;
	char *runner = NULL;

	char *vsa = (char *)malloc(pool_size);
	vsa = (char *)VSAInit(vsa, pool_size);
	
	runner = vsa;

	RUN_TEST("BeginHeaderTest1", -64 == *(ssize_t *)runner);
	runner += HEADER_SIZE;
	RUN_TEST("BeginHeaderTest2", MAGIC_NUM == *(size_t *)runner);

	runner += HEADER_SIZE + 64;
	RUN_TEST("EndHeaderTest1\t", 0 == *(ssize_t *)runner);
	runner += HEADER_SIZE;	
	RUN_TEST("EndHeaderTest2\t", MAGIC_NUM == *(size_t *)runner);

	free(vsa);

	return 0;
}

int VSAAllocTest()
{
	char *address = NULL;

	size_t pool_size = 100;

	char *vsa = (char *)malloc(pool_size);
	vsa = (char *)VSAInit(vsa, pool_size);

	address = VSAAlloc((vsa_t *)vsa, 30); /*padded to 32*/
	address -= 8;
	RUN_TEST("Alloc32TakenBytes\t", 32 == *(ssize_t *)address);
	address += (8 + 32);
	RUN_TEST("Alloc40FreeBytes\t", -40 == *(ssize_t *)address);

	address = VSAAlloc((vsa_t *)vsa, 15); /*padded to 16*/
	address -= 8;
	RUN_TEST("Alloc16TakenBytes\t", 16 == *(ssize_t *)address);
	address += (8 + 16);
	RUN_TEST("Alloc16FreeBytes\t", -16 == *(ssize_t *)address);
	
	address = VSAAlloc((vsa_t *)vsa, 12); /*padded to 16*/
	address -= 8;
	RUN_TEST("Alloc16TakenBytes\t", 16 == *(ssize_t *)address);
	address += (8 + 16);
	RUN_TEST("Alloc0FreeBytes\t\t", 0 == *(ssize_t *)address);
	
	address = VSAAlloc((vsa_t *)vsa, 50); /*no more space*/
	RUN_TEST("NoSpaceAllocResultsNULL", NULL == address);

	free(vsa);

	return 0;
}

int VSAAllocTestDEBUG()
{
	char *address = NULL;

	size_t pool_size = 100;

	char *vsa = (char *)malloc(pool_size);
	vsa = (char *)VSAInit(vsa, pool_size);

	address = VSAAlloc((vsa_t *)vsa, 30); /*padded to 32*/
	address -= 16;
	RUN_TEST("Alloc32TakenBytes\t", 32 == *(ssize_t *)address);
	address += (16 + 32);
	RUN_TEST("Alloc16FreeBytes\t", -16 == *(ssize_t *)address);

	address = VSAAlloc((vsa_t *)vsa, 7); /*padded to 8*/
	address -= 16;
	RUN_TEST("Alloc16TakenBytes\t", 16 == *(ssize_t *)address);
	address += (16 + 16);
	RUN_TEST("Alloc0FreeBytes\t\t", 0 == *(ssize_t *)address);
	
	address = VSAAlloc((vsa_t *)vsa, 50); /*no more space*/
	RUN_TEST("NoSpaceAllocResultsNULL", NULL == address);

	free(vsa);

	return 0;
}

int VSAFreeTest()
{
	char *address = NULL;
	ssize_t allocated_capacity = 0;
	size_t pool_size = 100;

	char *vsa = (char *)malloc(pool_size);
	vsa = (char *)VSAInit(vsa, pool_size);

				VSAAlloc((vsa_t *)vsa, 30); /*padded to 32*/
	address =	VSAAlloc((vsa_t *)vsa, 15); /*padded to 16*/
				VSAAlloc((vsa_t *)vsa, 12); /*padded to 16*/

	allocated_capacity = *((ssize_t *)(address - HEADER_SIZE));
	VSAFree(address);

	address -= HEADER_SIZE;
	RUN_TEST("IsAddressFree?\t\t", (-1)*ABS(allocated_capacity) == *(ssize_t *)address);

	address += HEADER_SIZE;
	RUN_TEST("IsAllocateToFreeAddress?", address == VSAAlloc((vsa_t *)vsa, 10));

	address -= HEADER_SIZE;
	RUN_TEST("IsCorrectTakenSpace?\t", 16 == *(ssize_t *)address);

	free(vsa);

	return 0;
}

int VSAFreeTestDEBUG()
{
	char *address = NULL;
	ssize_t allocated_capacity = 0;
	size_t pool_size = 100;
	char *vsa = NULL;

	vsa = 		(char *)malloc(pool_size);
	vsa = 		(char *)VSAInit(vsa, pool_size);

				VSAAlloc((vsa_t *)vsa, 30); /*padded to 32*/
	address =	VSAAlloc((vsa_t *)vsa, 15); /*padded to 16*/

	allocated_capacity = *((ssize_t *)(address - 2*HEADER_SIZE));
	VSAFree(address);

	address -= 2*HEADER_SIZE;
	RUN_TEST("IsAddressFree?\t\t", (-1)*ABS(allocated_capacity) == *(ssize_t *)address);

	address += 2*HEADER_SIZE;
	RUN_TEST("IsAllocateToFreeAddress?", address == VSAAlloc((vsa_t *)vsa, 10));

	address -= 2*HEADER_SIZE;
	RUN_TEST("IsCorrectTakenSpace?\t", 16 == *(ssize_t *)address);

	/*modify magic num*/
	address += HEADER_SIZE;
	*(size_t *)address = 99999;

	address += HEADER_SIZE;
	VSAFree(address);

	address -= 2*HEADER_SIZE;
	RUN_TEST("IsAddressFree?(no)\t", 16 == *(ssize_t *)address);
	
	free(vsa);

	return 0;
}

int VSADefragTest()
{
	char *address2 = NULL;
	char *address3 = NULL;

	char *address5 = NULL;
	char *address6 = NULL;
	char *address7 = NULL;

	char *address_a = NULL;

	size_t pool_size = 128;

	char *vsa = (char *)malloc(pool_size);
	vsa = (char *)VSAInit(vsa, pool_size);

			   VSAAlloc((vsa_t *)vsa, 8); 
	address2 = VSAAlloc((vsa_t *)vsa, 8); 
	address3 = VSAAlloc((vsa_t *)vsa, 8); 
			   VSAAlloc((vsa_t *)vsa, 8); 
	address5 = VSAAlloc((vsa_t *)vsa, 8);
	address6 = VSAAlloc((vsa_t *)vsa, 8); 
	address7 = VSAAlloc((vsa_t *)vsa, 8);

	VSAFree(address2);
	VSAFree(address3);

	VSAFree(address5);
	VSAFree(address6);
	VSAFree(address7);
	
	address_a = VSAAlloc((vsa_t *)vsa, 32);

	address2 -= 8;
	RUN_TEST("IsDataOnTheWayDefragged?", -24 == *(ssize_t *)address2);
	RUN_TEST("IsFirstFitAllocated?\t", address5 == address_a);

	free(vsa);

	return 0;
}

int VSADefragTestDEBUG()
{
	char *address2 = NULL;
	char *address3 = NULL;

	char *address5 = NULL;
	char *address6 = NULL;
	char *address7 = NULL;

	char *address_a = NULL;

	size_t pool_size = 256;

	char *vsa = (char *)malloc(pool_size);
	vsa = (char *)VSAInit(vsa, pool_size);

			   VSAAlloc((vsa_t *)vsa, 8); 
	address2 = VSAAlloc((vsa_t *)vsa, 8); 
	address3 = VSAAlloc((vsa_t *)vsa, 8); 
			   VSAAlloc((vsa_t *)vsa, 8); 
	address5 = VSAAlloc((vsa_t *)vsa, 8);
	address6 = VSAAlloc((vsa_t *)vsa, 8); 
	address7 = VSAAlloc((vsa_t *)vsa, 8);

	VSAFree(address2);
	VSAFree(address3);

	VSAFree(address5);
	VSAFree(address6);
	VSAFree(address7);
	
	address_a = VSAAlloc((vsa_t *)vsa, 40);

	address2 -= 16;
	RUN_TEST("IsDataOnTheWayDefragged?", -32 == *(ssize_t *)address2);
	
	RUN_TEST("IsFirstFitAllocated?\t", address5 == address_a);

	free(vsa);

	return 0;
}


int VSALargestChunkTest()
{
	size_t pool_size = 100;

	char *vsa = (char *)malloc(pool_size);
	vsa = (char *)VSAInit(vsa, pool_size);

	RUN_TEST("LargestChunk\t\t", -80 == *(ssize_t *)vsa);

	free(vsa);

	return 0;
}

int VSALargestChunkTestDEBUG()
{
	size_t pool_size = 100;

	char *vsa = (char *)malloc(pool_size);
	vsa = (char *)VSAInit(vsa, pool_size);
	
	RUN_TEST("LargestChunk\t\t", -64 == *(ssize_t *)vsa);

	free(vsa);

	return 0;
}
