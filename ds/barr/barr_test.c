/**********************************************************************
 *	This code is representing my test functions for DS_BArr           *
 *	Lab: OL-79                                		                  *
 *	Author: Yonatan Vologdin 										  *
 *	Date: 31.10.19													  *
 **********************************************************************/

#include <stdio.h>	/*printf*/
#include <string.h>	/*strcmp*/
#include "barr.h"	/*BArr structure*/

#define RESULT(condition) (condition) ? 1 : 0
#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define RUN_TEST(test_name, result) \
(printf("%s %s\n", test_name, (1 == (result)) ? KGRN"\tPassed"KNRM : KRED"\tFailed"KNRM))

int BArrSetAllTest();
int BArrResetAllTest();
int BArrSetBitTest();
int BArrSetOnTest();
int BArrSetOffTest();
int BArrIsOnTest();
int BArrIsOffTest();
int BArrCountOnTest();
int BArrCountOffTest();
int BArrCountOnLUTTest();
int BArrRotRTest();
int BArrRotLTest();
int BArrMirrorTest();
int BArrMirrorLUTTest();
int BArrToStringTest();
int BArrFlipTest();

int main()
{ 
	RUN_TEST("BArrSetAllTest\t", BArrSetAllTest());
	RUN_TEST("BArrResetAllTest", BArrResetAllTest());
	RUN_TEST("BArrSetBitTest\t", BArrSetBitTest());
	RUN_TEST("BArrSetOnTest\t", BArrSetOnTest());
	RUN_TEST("BArrSetOffTest\t", BArrSetOffTest());
	RUN_TEST("BArrIsOnTest\t", BArrIsOnTest());
	RUN_TEST("BArrIsOffTest\t", BArrIsOffTest());
	RUN_TEST("BArrCountOnTest\t", BArrCountOnTest());
	RUN_TEST("BArrCountOffTest", BArrCountOffTest());
	RUN_TEST("BArrCountOnLUTTest", BArrCountOnLUTTest());
	RUN_TEST("BArrRotRTest\t", BArrRotRTest());
	RUN_TEST("BArrRotLTest\t", BArrRotLTest());
	RUN_TEST("BArrMirrorTest\t", BArrMirrorTest());
	RUN_TEST("BArrMirrorLUTTest", BArrMirrorLUTTest());
	RUN_TEST("BArrToStringTest", BArrToStringTest());
	RUN_TEST("BArrFlipTest\t", BArrFlipTest());
	/*PrintLUTMask();	*/
	/*PrintLUTMirror();	*/
		
	return 0;
}

int BArrSetAllTest()
{
	bitarray arr1 = 0;
	return RESULT(0 == ~BArrSetAll(arr1));
}

int BArrResetAllTest()
{
	bitarray arr2 = 1;
	return RESULT(0 == BArrResetAll(arr2));
}

int BArrSetBitTest()
{
	bitarray arr3 = 64;
	return RESULT(0 == BArrSetBit(arr3, 6, 0));
}

int BArrSetOnTest()
{
	bitarray arr4 = 0;
	return RESULT(4 == BArrSetOn(arr4, 2));
}

int BArrSetOffTest()
{
	bitarray arr5 = 4;
	return RESULT(0 == BArrSetOff(arr5, 2));
}

int BArrIsOnTest()
{
	bitarray arr6 = 4;
	return RESULT(1 == BArrIsOn(arr6, 2));
}

int BArrIsOffTest()
{
	bitarray arr7 = 11; /*1011, 2nd bit is off*/
	return RESULT(1 == BArrIsOff(arr7, 2));
}

int BArrCountOnTest()
{
	bitarray arr8 = 11; /*1011, 2nd bit is off*/
	return RESULT(3 == BArrCountOn(arr8));
}

int BArrCountOffTest()
{
	bitarray arr8 = 11; /*1011, 2nd bit is off*/
	return RESULT(64-3 == BArrCountOff(arr8));
}

int BArrCountOnLUTTest()
{
	bitarray arr14 = 0X10880838421140CF; /*has 18 bits "on"*/
	return RESULT(18 == BArrCountOnLUT(arr14));
}

int BArrRotRTest()
{
	bitarray arr9 = 20; /*10100*/
	return RESULT(5 == BArrRotR(arr9, 2));
}

int BArrRotLTest()
{
	bitarray arr10 = 5; /*00101*/
	return RESULT(20 == BArrRotL(arr10, 2));
}

int BArrMirrorTest()
{
	bitarray arr11 = 536870912;
	return RESULT(17179869184 == BArrMirror(arr11));
}

int BArrMirrorLUTTest()
{
	bitarray arr15 = 70921457123526;
	return RESULT(7134337383327203328 == BArrMirrorLUT(arr15));
}

int BArrToStringTest()
{
	char buffer[65];
	char cmp[] = "0000000000000000000000000000000000010000000000000000000000001011";
	char *buffer_p = NULL;
	bitarray arr12 = 268435467;
	buffer_p = BArrToString(buffer ,arr12);
	
	return RESULT(0 == strcmp(buffer_p, cmp));
}

int BArrFlipTest()
{
	bitarray arr13 = 11; /*1011, 2nd bit is off*/
	return RESULT(7 == BArrFlip(BArrFlip(arr13, 2), 3));
}
