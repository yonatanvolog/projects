/**********************************************************************
 *	This code is representing my functions of DS_BArr                 *
 *	Lab: OL-79                                		                  *
 *	Author: Yonatan Vologdin 										  *
 *	Reviewer: Tamir Dayan										      *
 *	Date: 31.10.19													  *
 **********************************************************************/

#include <stdio.h>	/*printf*/
#include <assert.h>	/*assert*/
#include "barr.h"	/*BArr structure*/

#define BARR_SIZE 64
#define BYTE_SIZE 8

const size_t COUNT_ON_LUT[] = {0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4,
 1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 1, 2, 2, 3, 2, 3, 3, 4, 2, 3,
 3, 4, 3, 4, 4, 5, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 1, 2, 2, 3,
 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5,
 5, 6, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 3, 4, 4, 5, 4, 5, 5, 6,
 4, 5, 5, 6, 5, 6, 6, 7, 1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 2, 3,
 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5,
 4, 5, 5, 6, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 2, 3, 3, 4, 3, 4,
 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 4, 5, 5, 6, 5, 6, 6, 7, 5, 6, 
 6, 7, 6, 7, 7, 8};

const char MIRRORED_LUT[] =
   {0, -128, 64, -64, 32, -96, 96, -32, 16, -112, 80, -48, 48, -80, 112,
    -16, 8, -120, 72, -56, 40, -88, 104, -24, 24, -104, 88, -40, 56, -72,
    120, -8, 4, -124, 68, -60, 36, -92, 100, -28, 20, -108, 84, -44, 52,
    -76, 116, -12, 12, -116, 76, -52, 44, -84, 108, -20, 28, -100, 92, -36,
    60, -68, 124, -4, 2, -126, 66, -62, 34, -94, 98, -30, 18, -110, 82, -46,
    50, -78, 114, -14, 10, -118, 74, -54, 42, -86, 106, -22, 26, -102, 90,
   -38, 58, -70, 122, -6, 6, -122, 70, -58, 38, -90, 102, -26, 22, -106, 86,
   -42, 54, -74, 118, -10, 14, -114, 78, -50, 46, -82, 110, -18, 30, -98, 94,
   -34, 62, -66, 126, -2, 1, -127, 65, -63, 33, -95, 97, -31, 17, -111, 81,
   -47, 49, -79, 113, -15, 9, -119, 73, -55, 41, -87, 105, -23, 25, -103, 89,
   -39, 57, -71, 121, -7, 5, -123, 69, -59, 37, -91, 101, -27, 21, -107, 85, 
   -43, 53, -75, 117, -11, 13, -115, 77, -51, 45, -83, 109, -19, 29, -99, 93,
   -35, 61, -67, 125, -3, 3, -125, 67, -61, 35, -93, 99, -29, 19, -109, 83, 
   -45, 51, -77, 115, -13, 11, -117, 75, -53, 43, -85, 107, -21, 27, -101,
    91, -37, 59, -69, 123, -5, 7, -121, 71, -57, 39, -89, 103, -25, 23, -105,
    87, -41, 55, -73, 119, -9, 15, -113, 79, -49, 47, -81, 111, -17, 31, -97,
    95, -33, 63, -65, 127, -1};

/*Auxiliary functions*/
char ByteMirrorNoLoop(unsigned char x);
void PrintLUTMirror();
void PrintLUTCount();

bitarray BArrSetAll(bitarray arr)
{
	arr = 0XFFFFFFFFFFFFFFFF;
	
	return arr;
}

bitarray BArrResetAll(bitarray arr)
{
	arr = 0;
	
	return arr;
}

bitarray BArrSetBit(bitarray arr, size_t loc, int mod)
{
	assert(0 == mod || 1 == mod);	
	assert(loc < BYTE_SIZE);
		
	if (1 == mod)
	{
		arr = BArrSetOn(arr, loc);
	}
	else
	{
		arr = BArrSetOff(arr, loc);
	}
	return arr;
}

bitarray BArrSetOn(bitarray arr, size_t loc)
{
	arr |= (1 << loc);
		
	return arr;
}

bitarray BArrSetOff(bitarray arr, size_t loc)
{
	assert(loc < BYTE_SIZE);

	arr &= ~(1 << loc);
		
	return arr;
}

int BArrIsOn(bitarray arr, size_t loc)
{
	assert(loc < BYTE_SIZE);
			
	arr = arr & (1 << loc);
	arr >>= loc;
	
	return (int)arr;
}

int BArrIsOff(bitarray arr, size_t loc)
{
	assert(loc < BYTE_SIZE);

	arr &= (1 << loc);
	arr >>= loc;
	
	return !((int)arr);
}

size_t BArrCountOn(bitarray arr)
{
	size_t i = 0, count = 0;
	for (i = 0; i < BARR_SIZE; ++i)
	{
		if (arr & 1)
		{
			++count;
		}
		arr >>= 1;
	}
	return count;
}

size_t BArrCountOff(bitarray arr)
{
	size_t i = 0, count = 0;
	arr = ~arr;

	for (i = 0; i < BARR_SIZE; ++i)
	{
		if (arr & 1)
		{
			++count;
		}
		arr >>= 1;
	}
	return count;
}

size_t BArrCountOnLUT(bitarray arr)
{
	size_t i = 0, count = 0;
	bitarray byte_arr = 0, byte_mask = 255; /*mask = ...00011111111*/
	
	for (i = 0; i < BARR_SIZE/ BYTE_SIZE; ++i)
	{
		byte_arr = arr & byte_mask;
		count += COUNT_ON_LUT[byte_arr];
		arr >>= BYTE_SIZE;
	}
	return count;
}

bitarray BArrRotR(bitarray arr, size_t num)
{
	num %= BARR_SIZE;
	
	return ((arr >> num) | (arr << (BARR_SIZE - num)));
}

bitarray BArrRotL(bitarray arr, size_t num)
{
	num %= BARR_SIZE;
	
	return ((arr << num) | (arr >> (BARR_SIZE - num)));
}

bitarray BArrMirror(bitarray arr)
{
	bitarray rev_arr = 0;
	size_t i = 0, count = 0;
	
	for (i = 0; i < BARR_SIZE; ++i)
	{
		if (1 & arr)
		{
			rev_arr <<= 1; /*makes room for new bit*/
			rev_arr |= 1;	/*adds new bit*/
		}

		else
		{
			rev_arr <<= 1; /*"inserts" 0 bit*/
		}

		arr >>= 1;
		++count;
	}
	/*shift bits to correct loaction*/
	rev_arr <<= (BARR_SIZE - count); 
	arr = rev_arr;
	
	return arr;
}

bitarray BArrMirrorLUT(bitarray arr)
{
	size_t i = 0;
	bitarray byte_mask = 255, mirrored = 0; /*mask = ...00011111111*/
	
	for (i = 0; i < BARR_SIZE/ BYTE_SIZE - 1; ++i)
	{
		mirrored |= MIRRORED_LUT[arr & byte_mask];
		mirrored <<= BYTE_SIZE;
		arr >>= BYTE_SIZE;
	}
	return mirrored;
}

char* BArrToString(char* buffer ,bitarray arr)
{
	size_t i = 0;
	bitarray mask = 1, arr_tmp = arr;
	char* buffer_runner = buffer;
		
	assert(NULL != buffer);
		
	for (i = 0; i < BARR_SIZE; ++i)
	{
		mask = 1;
		mask <<= (BARR_SIZE - 1);
		
		arr_tmp = arr & mask;
		arr_tmp >>= (BARR_SIZE - 1);
		sprintf(buffer_runner ,"%ld", arr_tmp);
		arr <<= 1;
		++buffer_runner;
	}
	*buffer_runner = '\0';
	
	return buffer;	
}

bitarray BArrFlip(bitarray arr, size_t loc)
{
	assert(loc < BYTE_SIZE);
	
	if (BArrIsOff(arr, loc))
	{
		arr = BArrSetOn(arr, loc);
	}
	else
	{
		arr = BArrSetOff(arr, loc);
	}
	return arr;
}
char ByteMirrorNoLoop(unsigned char x)
{
	unsigned char m1  = 0x55;	/*binary: 0101...*/
	unsigned char m1r  = 0xaa;	/*binary: 1010...*/
	unsigned char m2  = 0x33;	/*binary: 00110011..*/
	unsigned char m2r  = 0xcc;	/*binary: 11001100..*/
	unsigned char m3  = 0x0f;	/*binary:  4 zeros,  4 ones ...*/
	unsigned char m3r  = 0xf0;	/*binary:  4 ones,  4 zeros ...*/
	unsigned char s1 = 0, s2 = 0;
	
	s1 = x & m1;
	s2 = x & m1r;
	
	s1 = s1 << 1;
	s2 = s2 >> 1;
	
	x = s1 | s2;
	
	s1 = x & m2;
	s2 = x & m2r;
	
	s1 = s1 << 2;
	s2 = s2 >> 2;
	
	x = s1 | s2;
	
	s1 = x & m3;
	s2 = x & m3r;
	
	s1 = s1 << 4;
	s2 = s2 >> 4;
	
	return (s1 | s2);
}

void PrintLUTMirror()
{
	bitarray i = 0;
	
	for (i = 0; i < 256; ++i)
	{
		printf("%x, ", ByteMirrorNoLoop(i));
	}
}

void PrintLUTCount()
{	
	bitarray i = 0;
	
	for (i = 0; i < 256; ++i)
	{
		printf("%ld, ", BArrCountOn(i));
	}
}
