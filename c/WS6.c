/**********************************************************************
 *	This code is representing my functions of WS6- Bitwise operations *
 *	Lab: OL-79 													      *
 *	Author: Yonatan Vologdin 										  *
 *	Reviewer: Sharon Rottner										  *
 *	Date: 15.10.19													  *
 **********************************************************************/

#include <stdio.h>		/*printf*/
#include <assert.h>		/*assert*/

/*This function recieves two numbers, sums them using bitwise operators only and returns result */
long BitwiseSum(long a, long b);

/*This function calculates x*(2^y) and prints the result*/
long Pow2(unsigned int x, unsigned int y);
void Pow2Test();

/*This function recieves unsigned int and returns 1 if it is power of 2, otherwise returns 0*/
int IsPowerOf2Loop(unsigned int num);
void IsPowerOf2LoopTest();

/*This function recieves unsigned int and returns true if it is power of 2, otherwise returns false*/
int IsPowerOf2NonLoop(unsigned long num);
void IsPowerOf2NonLoopTest();

/*This function recives a pointer to int and reverses the bits*/
void ByteMirrorLoop(int *num);
void ByteMirrorLoopTest();

/*This function recieves an unsigned char and reverses is bits*/
void ByteMirrorNoLoop(unsigned char *x);
void ByteMirrorNoLoopTest();

/*This function recieves a pointer to int and adds 1 to it*/
void AddOne(int *num);
void AddOneTest();

/*This function recieves an int and prints it if only 3 bits are "on"*/
void ThreeBitsOn(unsigned int num);

/*This function recieves a pointer to array of unsigned 
  numbers and prints only those who have 3 bits "on"*/
void ThreeBitsOnArr(unsigned int *arr, int len);
void ThreeBitsOnArrTest();

/*This function returns true if bits 2 and 6 are "on", otherwise returns false*/
int Function1(unsigned char ch);
void Function1Test();

/*This function returns true if eather bit 2 or 6 is "on", otherwise returns false*/
int Function2(unsigned char ch);
void Function2Test();

/*This funtion swaps between 3rd and 5th bits and returns the number*/
int Function3(unsigned char num);
void Function3Test();

/*This function returns the closest smaller number that's divisable by 16 without a remainder, if there is no such, returns -1*/
int SmallestNum(unsigned int num);
void SmallestNumTest();

/*This function recieves 2 pointers to int and swaps their values using bitwise operators*/
void Swap(int *x, int *y);
void SwapTest();

/*This function recieves an int and returns the number of set bits in it*/
int CountBitsSetLoop(int num);
void CountBitsSetLoopTest();

/*This function recieves an unsigned int and returns the number of set bits in it*/
int CountBitsSetNoLoop(unsigned char in);
void CountBitsSetNoLoopTest();

/*This function recieves a float and prints the bits inside it*/
void PrintFloat(float f);
void PrintFloatTest();

int main()
{
	Pow2Test();
	IsPowerOf2LoopTest();
	IsPowerOf2NonLoopTest();
	AddOneTest();
	ThreeBitsOnArrTest();
	ByteMirrorLoopTest();
	ByteMirrorNoLoopTest();
	Function1Test();
	Function2Test();
	Function3Test();
	SmallestNumTest();
	SwapTest();
	CountBitsSetLoopTest();
	CountBitsSetNoLoopTest();
	PrintFloatTest();
	
	return 0;
}

long BitwiseSum(long a, long b)
{
	while(0 != b)
	{
		long carry = a & b; /*used to determine carry, 1+1 is 2(aka 10)*/

		a = a ^ b;			/*used to "add", 1+0 = 1*/
		b = carry << 1;		/*shift carry 1 left because it will be applied to next number*/
	}

	return a;
}

long Pow2(unsigned int x, unsigned int y)
{	
	long base = 1, i = 0, res = 0;

	base = base << y; 						/*base = (2^y)*/

	for(; i < x; ++i)						/*res = (x*base)*/
	{
		res = BitwiseSum(res , base);
	}

	return res;
}

void Pow2Test()
{
	printf("Pow2Test 1:			%s\n",  Pow2(280, 4) == 4480 ? "Passed" : "Failed");
	printf("Pow2Test 2:			%s\n",  Pow2(290, 3) == 2320 ? "Passed" : "Failed");
}

int IsPowerOf2Loop(unsigned int num)
{
	while (0 != num)
	{
		if (1 == num) /*1 is 2^0, return true*/
		{	
			return 1;
		}
		if ((1 & num) == 1)
		{/*checks if num contains 1, if yes then It's an odd number*/
			return 0;
		}
		num = num >> 1; /*devides num by 2*/
	}
	
	return 0; 
}

void IsPowerOf2LoopTest()
{
	printf("IsPowerOf2LoopTest 1:		%s\n",  IsPowerOf2Loop(8) ? "Passed" : "Failed");
	printf("IsPowerOf2LoopTest 2:		%s\n",  !IsPowerOf2Loop(9) ? "Passed" : "Failed");
}	

int IsPowerOf2NonLoop(unsigned long num)
{
	return ((0 != num) && (0 == (num & (num - 1)))); 
}

void IsPowerOf2NonLoopTest()
{
	printf("IsPowerOf2NonLoopTest 1:	%s\n",  IsPowerOf2Loop(8) ? "Passed" : "Failed");
	printf("IsPowerOf2NonLoopTest 2:	%s\n",  !IsPowerOf2Loop(9) ? "Passed" : "Failed");
}

void AddOne(int *a)
{
	int b = 1;
	
	while(0 != b)
	{
		int carry = *a & b;
		*a = *a ^ b;	
		b = carry << 1;
	}
}

void AddOneTest()
{
	int num1 = 1, num2 = 2;
	
	AddOne(&num1);
	AddOne(&num2);
		
	printf("AddOneTest 1:			%s\n",  num1 == 2 ? "Passed" : "Failed");
	printf("AddOneTest 1:			%s\n",  num2 == 3 ? "Passed" : "Failed");
}

void ThreeBitsOn(unsigned int num)
{
	int count = 0;
	unsigned int num_print = num;

	while(0 != num)
	{
		if (num & 1)
		{
			++count;
		}
		num = num >> 1;
	}

	if (3 == count)
	{
		printf("%d\n",num_print);
	}
}	

void ThreeBitsOnArr(unsigned int *arr, int len)
{
	int i = 0;
	
	assert(arr);
	
	for (; i < len; ++i)
	{
		ThreeBitsOn(arr[i]);
	}
}

void ThreeBitsOnArrTest()
{
	unsigned int arr[] = {0, 1, 2, 3, 4, 5, 6, 8, 9, 10, 67};
	int arr_len = sizeof(arr);

	printf("ThreeBitsOnArrTest:		67 = ");
	ThreeBitsOnArr(arr, arr_len);
}

void ByteMirrorLoop(int *num)
{
	int rev_num = 0, count = 0;
	
	assert(num);
	
	while (0 != *num)
	{
		if (1 & *num)
		{
			rev_num = rev_num << 1; /*makes room for new bit*/
			rev_num = rev_num | 1;	/*adds new bit*/
		}

		else
		{
			rev_num = rev_num << 1; /*"inserts" 0 bit*/
		}

		*num = *num >> 1;
		++count;
	}
	/*shift bits to correct loaction*/
	rev_num = rev_num << (sizeof(int) - count); 
	*num = rev_num;
}

void ByteMirrorLoopTest()
{
	int num1 = 1; 	/*rev is 8*/
	int num2 = 10;	/*rev is 5*/
	
	ByteMirrorLoop(&num1);
	ByteMirrorLoop(&num2);
	
	printf("ByteMirrorLoopTest 1:		%s\n",  num1 == 8 ? "Passed" : "Failed");
	printf("ByteMirrorLoopTest 2:		%s\n",  num2 == 5 ? "Passed" : "Failed");
}

void ByteMirrorNoLoop(unsigned char *x)
{
	unsigned char m1  = 0x55;	/*binary: 0101...*/
	unsigned char m1r  = 0xaa;	/*binary: 1010...*/
	unsigned char m2  = 0x33;	/*binary: 00110011..*/
	unsigned char m2r  = 0xcc;	/*binary: 11001100..*/
	unsigned char m3  = 0x0f;	/*binary:  4 zeros,  4 ones ...*/
	unsigned char m3r  = 0xf0;	/*binary:  4 ones,  4 zeros ...*/
	unsigned char s1 = 0, s2 = 0;
	
	assert(x);
	
	s1 = *x & m1;
	s2 = *x & m1r;
	
	s1 = s1 << 1;
	s2 = s2 >> 1;
	
	*x = s1 | s2;
	
	s1 = *x & m2;
	s2 = *x & m2r;
	
	s1 = s1 << 2;
	s2 = s2 >> 2;
	
	*x = s1 | s2;
	
	s1 = *x & m3;
	s2 = *x & m3r;
	
	s1 = s1 << 4;
	s2 = s2 >> 4;
	
	*x = s1 | s2;
}

void ByteMirrorNoLoopTest()
{
	unsigned char num = 17; /*rev is 136*/

	ByteMirrorNoLoop(&num);
	printf("ByteMirrorNoLoopTest:		%s\n",  136 == num ? "Passed" : "Failed");
}

int Function1(unsigned char ch)
{	/*34 has bits 2 and 6 "on"*/
	return (34 == (ch & 34));
}

int Function2(unsigned char ch)
{	/*34 has bits 2 and 6 "on"*/
	return ((2 == (ch & 2)) || (32 == (ch & 32)));
}

int Function3(unsigned char num)
{
	int pos3 = 4;	/*4 has only 3rd bit "on"*/
	int pos5 = 16;	/*16 has only 5th bit "on"*/
	
	pos3 = pos3 & num;	/*extract bit from num*/
	pos5 = pos5 & num;
	
	pos3 = pos3 << 2;	/*pos3 now in position 5*/
	pos5 = pos5 >> 2;	/*pos5 now in position 3*/
	
	num = num & 235;	/*170 has all bits "on" except 3 and 5, places 0 at 3 and 5 positions*/
	num = num | pos3;	/*injection of new bits num*/
	num = num | pos5;
	
	return num;
}

void Function1Test()
{
	printf("Function1 Test:			%s\n",  Function1(35) ? "Passed" : "Failed");
}

void Function2Test()
{
	printf("Function2 Test:			%s\n",  Function2(34) ? "Passed" : "Failed");
}

void Function3Test()
{
	printf("Function3 Test:			%s\n",  4 == Function3(16) ? "Passed" : "Failed");
}

int SmallestNum(unsigned int num)
{	
	while ((0 != num) && (16 <= num))
	{
		if(IsPowerOf2NonLoop(num))
		{
			return num;
		}
		--num;
	}
	
	return -1;
}

void SmallestNumTest()
{
		printf("SmallestNumTest:		%s\n",  16 == SmallestNum(22) ? "Passed" : "Failed");
}

void Swap(int *x, int *y)
{
	assert(x && y);
	
	*x = *x ^ *y; /* x=x+y 						  => 5+2=7*/
	*y = *x ^ *y; /* y=x-y 						  => 7-2=5*/
	*x = *x ^ *y; /* x=difference between x and y => 7diff5=2*/
}

void SwapTest()
{
	int num1 = 5, num2 = 2;

	Swap(&num1, &num2);
	printf("SwapTest:			%s\n",  (2 == num1)&&(5 == num2) ? "Passed" : "Failed");
}

int CountBitsSetLoop(int num)
{
	int count = 0;

	while(0 != num)
	{
		if (1 == (num & 1))
		{
			++count;
		}
		num = num >> 1;
	}

	return count;
}	

void CountBitsSetLoopTest()
{
	printf("ThreeBitsSetLoopTest:		%s\n", 2 == CountBitsSetLoop(5) ? "Passed" : "Failed");
}

void PrintFloat(float f)
{
	int i = 0, size = sizeof(float)*8;
	unsigned int *f_pointer = (unsigned int *)&f;

	for(; i < size; ++i)
	{	/*2147483648 has only the leftmost bit of float "on"*/
		if (2147483648 == (*f_pointer & 2147483648))
		{
			printf("1");
		}
		else
		{
			printf("0");
		}
		
		*f_pointer = *f_pointer << 1;
	}
}

int CountBitsSetNoLoop(unsigned char in)
{
	unsigned char m1  = 0x55;	/*binary: 0101...*/
	unsigned char m2  = 0x33;	/*binary: 00110011..*/
	unsigned char m4  = 0x0f;	/*binary:  4 zeros,  4 ones ...*/
	unsigned char B,C = 0;
	unsigned char x = in;

	x = (x & (x << 1) & (m1 << 1)) | (m1 & (x ^ (x >> 1)));

	B = x & m2;
	C = (x >>  2) & m2;
	x = B ^ C ^ ((B & C) << 1);

	B = (x & m4 ) ^ ((x >>  4) & m4);
	C = (x & ((x >>  4) & m4)) << 1;
	x = B ^ C ^ ((B & C) << 1);
	
	return x;
}

void CountBitsSetNoLoopTest()
{
	printf("ThreeBitsSetNoLoopTest:		%s\n", 2 == CountBitsSetNoLoop(5) ? "Passed" : "Failed");
}

void PrintFloatTest()
{
	printf("PrintFloatTest (Check if numbers are same):\n");
	printf("\t\t");
	PrintFloat(100);
	printf("\n\t\t01000010110010000000000000000000\n");
}
