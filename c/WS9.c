/**********************************************************************
 *	This code is representing my functions of WS9- Word boundary      *
 *	Lab: OL-79 								(after code review update)*
 *	Author: Yonatan Vologdin 										  *
 *	Reviewer: Gal Salemon        									  *
 *	Date: 29.10.19													  *
 **********************************************************************/

#include <stdio.h>	/*printf*/
#include <assert.h>	/*assert*/
#include <string.h>	/*strcmp, memmove*/

#define JUMP_TO_LETTERS 7
#define LUT_SIZE 58
#define IS_LITTLE_ENDIAN (*(__uint16_t *)"\0\xff" > 0x100)

#define PRINT_TEST(test_name, result) \
(printf("%s %s\n", test_name, (0 == (result)) ? "\tPassed" : "\tFailed"))

typedef size_t word;
#define	word_size (unsigned int)sizeof(word)

/*	This function recieves des (pointer to the object or block of memory that needs to be set),
	ch (value that is copied), and sets num bytes as ch in des.
	Returns pointer void pointer to changed memory location.
*/
void *MyMemset(void *des, int ch, size_t num);
void ByteMemset(char *des, int ch, size_t num);
void MyMemsetTest();

/*	This function recieves des (pointer to the object or block of memory),
 	const source (pointer to the object or block of memory that needs to be copied to des),
	and copies num bytes as from source into des.
	Returns pointer void pointer to des.
*/
void *MyMemcpy(void *des, const void *source, size_t num);
void WordMemcpy(size_t *des, const size_t *source, size_t num);
void ByteMemcpy(char *des, char *source, size_t num);
void MyMemcpyTest();

/*	This function recieves des (pointer to the object or block of memory that needs to be set),
	ch (value that is copied), and sets num bytes as ch in des.
	Returns pointer void pointer to changed memory location.
*/
void *MyMemmove(void *des, const void *source, size_t num);
void *RvMyMemcpy(void *des, const void *source, size_t num);
void RevWordMemcpy(size_t *des, const size_t *source, size_t num);
void RevByteMemcpy(char *des, char *source, size_t num);
void MyMemmoveTest();

/*	This function receives 2 pointers to chars and swaps them by reference
*/
void CharSwap(char *a, char *b);
/*	This function receives an int and pointer to pointer to string,
	Converts the int to a string and returns a pointer to it.
*/
char *MyitoaBase10(int value, char *str);
void MyitoaBase10Test();

/*	This function receives a number in a string, and returns the number as an int.
	The function will ignore any white spaces before the number, but after that will 
	refer to any character as a number. If the number is illigal - retuns 0.
*/
int MyatoiBase10(const char *str);
void MyatoiBase10Test();

/*	This function recieves an int, and converts it into a number with a given base.
	The new number is pointed to by str, which the function returns.
*/
char *Myitoa(int value, char *str, int base);
void MyitoaTest();

/*	This function receives a pointer to number in "base" represented in string,
	and string lengh. Returns said number in base 10 as int. 
*/
int Myatoi(const char *str, int str_len, int base);
void MyatoiTest();

/*	This function receives 3 arrays and then it's sizes, traverses them and prints out
	only letters that appear in both arr1 & arr2 but not in arr3.
*/
void PrintLetters(int *arr1, int *arr2, int *arr3, int len1, int len2, int len3);
void PrintLettersTest();

/*	This function checks if the system is "little endian".
	Returns 1 if so, and 0 if it's "Big endian"
*/
int IsLittleEndian(void);
void IsLittleEndianFuncTest();
void IsLittleEndianMacroTest();

int main()
{
	MyMemsetTest();
	MyMemcpyTest();
	MyMemmoveTest();
	MyatoiBase10Test();
	MyitoaBase10Test();
	MyatoiTest();
	MyitoaTest();
	PrintLettersTest();
	IsLittleEndianFuncTest();
	IsLittleEndianMacroTest();
		
	return 0;
}

void *MyMemset(void *des, int ch, size_t num)
{
	size_t *des_runner = des;
	size_t chunk = ch;
	unsigned int i = 0;
	unsigned start_loop = word_size - ((size_t)des_runner % word_size);
	unsigned chunk_loop = 0;
	unsigned end_loop = 0;

	assert(NULL != des);

	/*means already aligned, no need for start loop*/
	if (start_loop == word_size)
	{
		start_loop = 0;
	}
	chunk_loop = (num - start_loop) / word_size;
	end_loop = num - start_loop - (chunk_loop * word_size);

	/*loop to create word sized chunk*/
	for (i = 0; i < word_size ; ++i)
	{
		chunk = chunk << word_size;
		chunk = chunk | ch;
	}
	/*copy start tail to des*/
	ByteMemset((char *)des_runner, ch, start_loop);
	des_runner = des_runner + start_loop;

	/*copy chunk to des*/
	for (i = 0; i < chunk_loop; ++i, ++des_runner)
	{	
		*des_runner = chunk;
	}
	/*copy end tail to des*/
	ByteMemset((char *)des_runner, ch, end_loop);

	return des;
}

void ByteMemset(char *des, int ch, size_t num)
{
	size_t i = 0;

	assert(NULL != des);

	for (i = 0; i < num; ++i)
	{
		*(des + i) = ch;
	}
}

void *MyMemcpy(void *des, const void *source, size_t num)
{
	char *des_runner = des;
	unsigned int start_loop = word_size - ((size_t)des_runner % word_size);
	unsigned int mid_loop = 0;
	unsigned int end_loop = 0;
	
	assert(NULL != source);
	assert(NULL != des);
	
	/*means already aligned, no need for start loop*/
	if (start_loop == word_size)
	{
		start_loop = 0;
	}
	mid_loop = (num - start_loop) / word_size;
	end_loop = num - start_loop - (mid_loop * word_size);

	/*copy start tail to des*/
	ByteMemcpy((char *)des_runner, (char *)source, start_loop);
	
	/*move pointers to start of mid address*/
	des_runner = des_runner + start_loop; 
	source = (char *)source + start_loop;

	/*copy mid section to des*/
	WordMemcpy((size_t *)des_runner, source, mid_loop);
	
	/*move pointers to start of end address*/
	des_runner = des_runner + mid_loop * word_size; 
	source = (char *)source + mid_loop * word_size;
	
	/*copy end tail to des*/
	ByteMemcpy((char *)des_runner, (char *)source, end_loop);

	return des;
}

void WordMemcpy(size_t *des, const size_t *source, size_t num)
{	
	size_t i = 0;
	
	assert(NULL != source);
	assert(NULL != des);
	
	for (i = 0; i < num; ++i)
	{
		*(des + i) = *(source + i);
	}
}

void ByteMemcpy(char *des, char *source, size_t num)
{	
	size_t i = 0;
	
	assert(NULL != source);
	assert(NULL != des);
	
	for (i = 0; i < num; ++i)
	{
		*(des + i) = *(source + i);
	}
}

void *MyMemmove(void *des, const void *source, size_t num)
{	
	assert(NULL != source);
	assert(NULL != des);

	if (des > source)
	{
		RvMyMemcpy(des, source, num);
	}
	else if (source > des)
	{
		MyMemcpy(des, source, num);
	}
	
	return des;
}

void *RvMyMemcpy(void *des, const void *source, size_t num)
{
	char *des_runner = des;
	unsigned int start_loop = word_size - ((size_t)des_runner % word_size);
	unsigned int mid_loop = 0;
	unsigned int end_loop = 0;
	
	assert(NULL != source);
	assert(NULL != des);

	/*means already aligned, no need for start loop*/
	if (start_loop == word_size)
	{
		start_loop = 0;
	}
	mid_loop = (num - start_loop) / word_size;
	end_loop = num - start_loop - (mid_loop * word_size);

	/*move pointers to start of end address*/
	des_runner = des_runner + start_loop + (mid_loop * word_size);
	source = (char *)source + start_loop + (mid_loop * word_size);

	/*copy end tail to des*/
	RevByteMemcpy((char *)des_runner, (char *)source, end_loop);
	
	/*move pointers to start of mid address*/
	des_runner = des_runner - (mid_loop * word_size); 
	source = (char *)source - (mid_loop * word_size);

	/*copy mid section to des*/
	RevWordMemcpy((size_t *)des_runner, source, mid_loop);
	
	/*move pointers to start of start address*/
	des_runner = des_runner - start_loop; 
	source = (char *)source - start_loop;
	
	/*copy start tail to des*/
	RevByteMemcpy((char *)des_runner, (char *)source, start_loop);

	return des;
}

void RevWordMemcpy(size_t *des, const size_t *source, size_t num)
{	
	size_t i = 0;
	
	assert(NULL != source);
	assert(NULL != des);
	
	for (i = 0; i < num; ++i)
	{
		*(des + num - 1 - i) = *(source + num - 1 - i);
	}
}

void RevByteMemcpy(char *des, char *source, size_t num)
{	
	size_t i = 0;
	
	assert(NULL != source);
	assert(NULL != des);
	
	for (i = 0; i < num; ++i)
	{
		*(des + num - 1 - i) = *(source + num - 1 - i);
	}
}
void MyMemsetTest()
{
	char string1[]  = "01234567891011121314151617181920abcdefghigklmnop"; 
	char string2[] = "01234567891011121314151617181920abcdefghigklmnop";
	
	MyMemset(string1, 'a', 3); 
	memset(string2, 'a', 3); 
	PRINT_TEST("MyMemsetTest:		", strcmp(string1, string2));
}

void MyMemcpyTest()
{
	char string1[]  = "01234567891011121314151617181920abcdefghigklmnop"; 
	char string2[]  = "01234567891011121314151617181920abcdefghigklmnop"; 
	char string3[] = "copy this";
	
	MyMemcpy(string1, string3, 8); 
	memcpy(string2, string3, 8); 
	PRINT_TEST("MyMemcpyTest:		", strcmp(string1, string2));
}

void MyMemmoveTest()
{
	char string1[]  = "01234567891011121314151617181920abcdefghigklmnop"; /*48 characters*/
	char string2[] = "01234567891011121314151617181920abcdefghigklmnop";
	
	memmove(string1 + 7, string1 + 9, 8);
	MyMemmove(string2 + 7, string2 + 9, 8);
	PRINT_TEST("MyMemmoveTest1(dest < source):", strcmp(string1, string2));
	
	memmove(string1 + 7, string1 + 9, 14);
	MyMemmove(string2 + 7, string2 + 9, 14);
	PRINT_TEST("MyMemmoveTest2(dest < source):", strcmp(string1, string2));
	
	memmove(string1 + 7, string1 + 9, 28);
	MyMemmove(string2 + 7, string2 + 9, 28);
	PRINT_TEST("MyMemmoveTest3(dest < source):", strcmp(string1, string2));
	
	memmove(string1 + 9, string1 + 7, 8);
	MyMemmove(string2 + 9, string2 + 7, 8);
	PRINT_TEST("MyMemmoveTest4(source < dest):", strcmp(string1, string2));
	
	memmove(string1 + 9, string1 + 7, 10);
	MyMemmove(string2 + 9, string2 + 7, 10);
	PRINT_TEST("MyMemmoveTest5(source < dest):", strcmp(string1, string2));
	
	memmove(string1 + 9, string1 + 7, 17);
	MyMemmove(string2 + 9, string2 + 7, 17);
	PRINT_TEST("MyMemmoveTest6(source < dest):", strcmp(string1, string2));
	
	memmove(string1 + 9, string1 + 7, 29);
	MyMemmove(string2 + 9, string2 + 7, 29);
	PRINT_TEST("MyMemmoveTest7(source < dest):", strcmp(string1, string2));
	
	memmove(string1 + 0, string1 + 0, 29);
	MyMemmove(string2 + 0, string2 + 0, 29);
	PRINT_TEST("MyMemmoveTest8(source = dest):", strcmp(string1, string2));
}

void CharSwap(char *a, char *b)
{
	char tmp = *a;
	*a = *b;
	*b = tmp;
}

char *MyitoaBase10(int value, char *str)
{
	int i = 0, j = 0;	
	int base = 0;

	assert(NULL != str);
	
	while (0 < value)
	{
		str[base] = (value % 10) + '0';
		value = value / 10;
		++base;
	}
	
	for (j = base - 1; i < base/2; --j, ++i)
	{
		CharSwap(str + j, str + i);
	}
	str[base] = '\0';

	return str;
}

int MyatoiBase10(const char *str)
{
	int i = 0, num = 0, num_sign = 1;
	int str_len = strlen(str);

	assert(NULL != str);

	while (' ' == str[i])
	{
		++i;
		if(i > str_len)
		{
			return 0;
		}
	}
	
	if ('-' == str[i])
	{
		num_sign = -1;
		++i;	
	}
	
	else if ('+' == str[i])
	{	
		++i;
	}

	while(str[i] > '0' && str[i] < '9')
	{
		while(i < str_len)
		{
			num = num + str[i] - '0';
			num = num * 10;
			++i;
		}
		num = num / 10;
		return num_sign * num;
	}

	return 0;
}

char *Myitoa(int value, char *str, int base)
{
	int  i = 0, k = 0, j = 0, digit = 0;
	
	assert(NULL != str);
	
	while (0 < value)
	{
		digit = value % base;
		if (9 < digit)
		{

			digit = digit + JUMP_TO_LETTERS; /*jump to letters in ascii table*/
		}
		str[i] = digit + '0';
		value = value / base;
	++i;
	}
	
	k = 0;
	/*reverse str*/
	for (j = i - 1; k < i/2; --j, ++k)
	{
		CharSwap(str + j, str + k);
	}
	str[i] = '\0';
 
	return str;
}

int Myatoi(const char *str, int str_len, int base)
{
	int i = 0, index = 0, k = str_len - 2;
	int multiplier = 1, num_sign = 1;
	long res_dec = 0;

	assert(NULL != str);

	while (' ' == str[i])
	{
		++i;
		if(i > str_len)
		{
			return 0;
		}
	}
	if ('-' == str[i])
	{
		num_sign = -1;
		++i;
	}
	else if ('+' == str[i])
	{	
		++i;
	}
	/*while character in number or capital letter*/
	while((str[k] >= '0' && str[k] <= '9') || (str[k] >= 'A' && str[k] <= 'Z'))
	{
		if(str[k] >= '0' && str[k] <= '9')
		{
			res_dec = res_dec + (str[k] - '0') * multiplier;
		}
		
		else if(str[k] >= 'A' && str[k] <= 'Z')
		{
			num_sign = 1;
			res_dec = res_dec + (str[k] - 'A' + 10) * multiplier;
		}
								
		--k;
		++index;
		multiplier *= base;
		if(k + 1 == i)
		{
			return num_sign * res_dec;
		}
	}
	return 0;
}

void MyatoiBase10Test()
{
	int conv = 0;	
	char string1[] = "     -1997";
	conv = MyatoiBase10(string1);
	PRINT_TEST("MyatoiBase10Test:	", ((-1997 == conv) - 1));
}

void MyitoaBase10Test()
{
	int num = 161;
	char *str1 = "161";
	char str2[30];
	MyitoaBase10(num, str2);
	PRINT_TEST("MyitoaBase10Test:	", strcmp(str1, str2));
}

void MyatoiTest()
{
	int conv = 0;	
	char string2[] = "         -A1B2C3"; 
	conv = Myatoi(string2, sizeof(string2)/sizeof(char), 16);
	PRINT_TEST("MyatoiTest:		", ((10597059 == conv) - 1));
}

void MyitoaTest()
{
	int num = 161;
	char *num_binary = "10100001";
	char *num_hex = "A1";
	char string[30];
	Myitoa(num, string, 2);
	PRINT_TEST("MyitoaTest1(binary):	", strcmp(string, num_binary));
	Myitoa(num, string, 16);
	PRINT_TEST("MyitoaTest2(hex):	", strcmp(string, num_hex));
}

void PrintLetters(int *arr1, int *arr2, int *arr3, int len1, int len2, int len3)
{
	int i = 0;
	int letters_lut[LUT_SIZE] = {0}; /*from 'A' - 65 to 'z' - 122*/
	
	assert(NULL != arr1);
	assert(NULL != arr2);
	assert(NULL != arr3);
	
	for(i = 0; i < len1; ++i)
	{
		letters_lut[arr1[i] - 'A'] += 1;
	}
	
	for(i = 0; i < len2; ++i)
	{
		letters_lut[arr2[i] - 'A'] += 1;
	}
	
	for(i = 0; i < len3; ++i)
	{
		letters_lut[arr3[i] - 'A'] = 0;
	}
	for(i = 0; i < LUT_SIZE; ++i)
	{	
		if(2 <= letters_lut[i])
		{
			printf("%c", i + 'A');
		}
	}
	printf("\n");
}

void PrintLettersTest()
{
	int arr1[] = {'a', 'b', 't','c', 'd', 'r'}; 
	int arr2[] = {'e', 'f', 'r', 't','a', 'b'};
	int arr3[] = {'e', 'z', 'a','t', 'k'};
	printf("PrintLettersTest:		br == ");
	PrintLetters(       arr1, 					  arr2, 					arr3, 
	sizeof(arr1)/sizeof(int), sizeof(arr2)/sizeof(int), sizeof(arr3)/sizeof(int));
}


int IsLittleEndian(void)
{
	int num = 1;
	if(1 == *((char *)&num))
	{
		return 1;
	}
	return 0;
}

void IsLittleEndianFuncTest()
{
	printf("IsLittleEndianFuncTest:		");
	
	if(IsLittleEndian())
	{
		printf("System is Little Endian\n");
	}
	else
	{
		printf("System is Big Endian\n");
	}
}

void IsLittleEndianMacroTest()
{
	printf("IsLittleEndianMacroTest:	");
	
	if(IS_LITTLE_ENDIAN)
	{
		printf("System is Little Endian\n");
	}
	else
	{
		printf("System is Big Endian\n");
	}	
}
