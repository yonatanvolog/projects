/**********************************************************************
 *	This code is representing my functions of WS8- Structs            *
 *	Lab: OL-79 													      *
 *	Author: Yonatan Vologdin 										  *
 *	Reviewer: Eyal Finkelshtein										  *
 *	Date: 23.10.19													  *
 **********************************************************************/

#include <stdio.h>	/*printf*/
#include <string.h>	/*strcpy*/
#include <stdlib.h>	/*malloc, free*/
#include <assert.h>	/*assert*/

#define ARR_SIZE 3
#define STRING_BUFFER 20

#define MAX2(X,Y) ((X > Y) ? X : Y)
#define MAX3(X,Y,Z) (MAX2(MAX2(X,Y), MAX2(Y,Z)))

#define SIZEOF_VAR(var) ((char *)(&var + 1) - (char *)&var)
#define SIZEOF_TYPE(X)  ((X*)0+1)

#define PRINT_TEST(test_name, result) \
(printf("%s %s\n", test_name, (1 == (result)) ? "\tPassed" : "\tFailed"))

typedef struct Node
{
	enum type {INT, FLOAT, STRING} data_type;
	void *data;
	void (*PrintFunc)();
	void (*AddFunc)();
} NODE;

void PrintInt(void *data);
void AddInt(void **data, int add);
void PrintFloat(void *data);
void AddFloat(void **data, int add);
void PrintString(void *data);
void AddString(void **data, int add);
void PrintArr(NODE *arr);
void AddToArr(NODE *arr, int add);
void Cleanup(NODE *arr);
void ArrayElementsTest();
void MacrosTest();

int main()
{
	ArrayElementsTest();
	MacrosTest();
	
	return 0;
}

void PrintInt(void *data)
{
	assert(*((int*)&data));
	
	printf("int:		%d\n", *((int*)&data));
}

void AddInt(void **data, int num_to_add)
{
	assert(data);
	
	*((int*)data) += num_to_add;
}

void PrintFloat(void *data)
{
	assert(*((float*)&data));
	
	printf("float:		%f\n", *((float*)&data));
}

void AddFloat(void **data, int num_to_add)
{
	assert(*((float*)&data));
	
	*((float*)data) += num_to_add;
}

void PrintString(void *data)
{
 	assert(data);
	
	printf("string:		%s\n", (char *)data);
}

void AddString(void **data, int num_to_add)
{
	assert(data);

	sprintf(*data, "%s%d", (char *)*data, num_to_add);
}

void PrintArr(NODE *arr)
{
	int i = 0;
	
	assert(arr);
	
	for (; i < ARR_SIZE; ++i)
	{
		printf("arr[%d]-", i);
		arr[i].PrintFunc(arr[i].data);
	}	
}

void AddToArr(NODE *arr, int num_to_add)
{
	int i = 0;

 	assert(arr);

	for (; i < ARR_SIZE; ++i)
	{
		arr[i].AddFunc(&arr[i].data, num_to_add);
	}
}

void Cleanup(NODE *arr)
{
	int i = 0;

 	assert(arr);

	for (; i < ARR_SIZE; ++i)
	{
		if(STRING == arr[i].data_type)
		{
			free(arr[i].data);
		}
	}
}

void ArrayElementsTest()
{
	int num_to_add = 10;
	char word[] = "Hello World";
	NODE arr[ARR_SIZE]; 
	
	arr[0].data_type = INT;
	*((int *)&arr[0].data) = 12;
	arr[0].PrintFunc = PrintInt;
	arr[0].AddFunc = AddInt;
	
	arr[1].data_type = FLOAT;
	*((float *)&arr[1].data) = 4.2;
	arr[1].PrintFunc = PrintFloat;
	arr[1].AddFunc = AddFloat;
	
	arr[2].data_type = STRING;
	arr[2].data = (void *)malloc(sizeof(word) + STRING_BUFFER); 
	strcpy((char *)arr[2].data, word);
	arr[2].PrintFunc = PrintString;
	arr[2].AddFunc = AddString;
	
	PrintArr(arr);
	AddToArr(arr, num_to_add);
	PrintArr(arr);
	Cleanup(arr);
}

void MacrosTest()
{
	int int_num;
	char *char_ptr;	
	NODE my_node;

	PRINT_TEST("Max2Test:	", 51 == MAX2(51,50));
	PRINT_TEST("Max3Test:	", 51 == MAX3(51,50,21));
	PRINT_TEST("SIZEOF_VAR Test1:", SIZEOF_VAR(int_num) == sizeof(int_num));
	PRINT_TEST("SIZEOF_VAR Test2:", SIZEOF_VAR(char_ptr) == sizeof(char_ptr));
	PRINT_TEST("SIZEOF_VAR Test3:", SIZEOF_VAR(my_node) == sizeof(my_node));
	PRINT_TEST("SIZEOF_TYPE Test1:", (long)SIZEOF_TYPE(int) == (long)sizeof(int));			
	PRINT_TEST("SIZEOF_TYPE Test2:", (long)SIZEOF_TYPE(char) == (long)sizeof(char));			
	PRINT_TEST("SIZEOF_TYPE Test3:", (long)SIZEOF_TYPE(NODE) == (long)sizeof(NODE));			
}
