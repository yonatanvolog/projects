/**********************************************************************
 *	This code is representing my tests of DS_CBuff RC1   	          *
 *	Lab: OL-79                                		                  *
 *	Author: Yonatan Vologdin 										  *
 *	Reviewer: Eyal Finkelshtein   								      *
 *	Date: 10.11.19													  *
 **********************************************************************/
#include <stdio.h>	/*printf*/
#include <stdlib.h>	/*free*/
#include <string.h> /*strncmp*/

#include "cbuff.h"	/*circular buffer data structure*/

#define RESULT(condition) ((condition) ? 1 : 0)

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define RUN_TEST(test_name, result) \
(printf("%s %s\n", test_name, (1 == (result)) ? KGRN"\tPassed"KNRM : KRED"\tFailed"KNRM))

struct CB /*in order to dereference pointers of this type*/
{
	size_t capacity;
	char *r_ptr;
	char *w_ptr;
	char begin[1];
};

int CBuffCreateDestroyTest();
int CBuffCreateDestroyTest();
int CBuffCapacityTest();
int CBBuffSpaceLeftIsEmptyTest();
int CBuffWriteTest();
int CBuffWriteAndReadTest();

int main()
{
	RUN_TEST("CBuffCreateDestroyTest:", CBuffCreateDestroyTest());
	RUN_TEST("CBuffWriteTest:\t\t", CBuffWriteTest());
	RUN_TEST("CBuffCapacityTest:\t", CBuffCapacityTest());
	RUN_TEST("CBBuffSpaceLeftIsEmptyTest:", CBBuffSpaceLeftIsEmptyTest());
	RUN_TEST("CBuffWriteAndReadTest:\t", CBuffWriteAndReadTest());

	return 0;
}

int CBuffWriteAndReadTest()
{
	int res = 0, total = 0;
	
	char src_string[] = "123456789";
	char des_string[50] = {0};
	cbuff_t *buffer = CBuffCreate(6);

	total +=res;
	res = 0;
	res += RESULT(6 == CBuffWrite(buffer, src_string, 6));
	res += RESULT(4 == CBuffRead(buffer ,des_string, 4));
	res += RESULT(0 == strncmp(des_string, src_string, 3));
	res += RESULT(4 == CBuffSpaceLeft(buffer));
	RUN_TEST("Write&Read Test1:", (4 == res));
	
	total +=res;
	res = 0;
	res += RESULT(3 == CBuffWrite(buffer, src_string + 6, 3));
	res += RESULT(2 == CBuffRead(buffer ,des_string + 4, 2));
	res += RESULT(0 == strncmp(des_string, src_string, 5));
	res += RESULT(3 == CBuffSpaceLeft(buffer));
	RUN_TEST("Write&Read Test2:", (4 == res));
			
	total +=res;
	res = 0;		
	res += RESULT(3 == CBuffRead(buffer ,des_string + 6, 1000));
	res += RESULT(0 == strncmp(des_string, src_string, 9));
	res += RESULT(6 == CBuffSpaceLeft(buffer));
	RUN_TEST("Write&Read Test3:", (3 == res));

	total +=res;
	res = RESULT(11 == total);
	
	CBuffDestroy(buffer);
	
	return res;
}

int CBuffWriteTest()
{
	int res = 0;
	
	char string[] = "123456789";
	cbuff_t *buffer = CBuffCreate(7);

	res += RESULT(6 == CBuffWrite(buffer, string, 6));
	res += RESULT(1 == CBuffSpaceLeft(buffer));
	res += RESULT(1 == CBuffWrite(buffer, string + 6, 6));
	res += RESULT(0 == CBuffSpaceLeft(buffer));
	res += RESULT(0 == strncmp(buffer->begin, string, 7));
	
	res = RESULT(5 == res);
	
	CBuffDestroy(buffer);
	
	return res;
}

int CBBuffSpaceLeftIsEmptyTest()
{
	int res = 0;
	cbuff_t *buffer = CBuffCreate(5);
	res += RESULT(5 == CBuffSpaceLeft(buffer));
	
	buffer->w_ptr += 5;
	buffer->r_ptr += 1;
	res += RESULT(1 == CBuffSpaceLeft(buffer));

	buffer->r_ptr += 3;
	res += RESULT(4 == CBuffSpaceLeft(buffer));

	buffer->r_ptr += 1;
	res += RESULT(1 == CBuffIsEmpty(buffer));
	
	buffer->w_ptr -= 2 - buffer->capacity; /*returns to start and writes 2*/
	res += RESULT(2 == CBuffSpaceLeft(buffer));

	res = RESULT(5 == res);
	CBuffDestroy(buffer);

	return res;
}


int CBuffCreateDestroyTest()
{
	int res = 0;
	cbuff_t *buffer = CBuffCreate(5);
	res = RESULT((5 == buffer->capacity) && (buffer->w_ptr == buffer->r_ptr));
	CBuffDestroy(buffer);

	return res;
}

int CBuffCapacityTest()
{
	int res = 0;
	cbuff_t *buffer = CBuffCreate(5);
	res = RESULT((5 == CBuffCapacity(buffer)));
	free(buffer);

	return res;
}
