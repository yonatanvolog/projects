/********************************************************************
 *	This is code representing my functions of WS2					*
 *	Lab: OL-79 														*
 *	Author: Yonatan Vologdin 										*
 *	Date: 03.10.19													*
 ********************************************************************/
	/*printf("LargeNumbersTest 1:	%s\n", *string12 == "1000" ? "Passed" : "Failed");*/
#include <stdio.h> 	/*for printf function*/
#include <assert.h> /*for assert function*/
#include <stdlib.h> /*for calloc function*/
#include <string.h>

#define CHAR_CONV 48 /*48 in ASCII represents '0'*/

/*	This function recieves 2 large numbers as strings, adds them up and returns the
 *	result in a in a pointer to a new string.	
 */
char *LargeNumbers(char *num1, char *num2);
void LargeNumbersTest();

/*	This function recieves a string and returns 1 is it's palindrome
 *	or 0 if it's not palindrome.
 */
int IsPalindrome(const char *str);
void IsPalindromeTest();

/*	This function recieves an int, prints "BOOM" if it includes the 
 *	digit 7 or devidable by 7. Otherwise prints the int it recieved.
 */
int SevenBoom(int num);
void SevenBoomTest();

/* This function swaps to pointers to int
 */
void swap(int **a, int **b);
void swapTest();

/*	This function revieces a string and removes all spaces from the 
 *	beginning and the end, and also replaces repeating spaces between words with one.
 */
void RmSpaces(char *str);
void RmSpacesTest();

int main()
{
	LargeNumbersTest();
	IsPalindromeTest();
	SevenBoomTest();
	RmSpacesTest();
	swapTest();

	return 0;
}

char *LargeNumbers(char *num1, char *num2)
{
	char *new_num;	
	int num1_size = 0, num2_size = 0, new_num_size = 0;	
	int move_next = 0, sum = 0, i = 0;

	assert(*num1 && *num2); /*make sure both numbers are not NULL*/

	while(*(num1 + num1_size)) /*measure num1 size*/
	{
		++num1_size;
	}

	while(*(num2 + num2_size)) /*measure num2 size*/
	{
		++num2_size;
	}

	if (num1_size < num2_size) 	/*conditions to decide new string size*/
	{
		new_num_size = num2_size + 1;
	}	

	else
	{
		new_num_size = num1_size + 1;
	}
	/*allocating memmory for new string, 1 extra for extra possible
	  character and another one for terminating NULL character*/
	new_num = calloc(new_num_size + 2, sizeof(char)); 

	*(new_num + new_num_size + 1) = '\0'; 			  /*setting up terminating character*/
	i = new_num_size -1;
	
	while ((0 < num1_size) && (0 < num2_size))		  /*performing arithmetics on both numbers*/
	{			
			sum = (*(num1 + num1_size-1)- CHAR_CONV) + (*(num2 +num2_size-1)- CHAR_CONV) + move_next;
			move_next = sum / 10;
			*(new_num + i ) = (char)((sum % 10) + CHAR_CONV);	
			--num1_size;
			--num2_size;
			--i;
	}

	while (0 < num1_size)							  /*performing arithmetics on the left number*/	
	{			
			sum = (*(num1 + num1_size-1)- CHAR_CONV) + move_next;
			move_next = sum / 10;
			*(new_num + i ) = (char)((sum % 10) + CHAR_CONV);	
			--num1_size;
			--i;
	}

	while (0 < num2_size)	
	{		
			sum = (*(num2 + num2_size-1)- CHAR_CONV) + move_next;
			move_next = sum / 10;
			*(new_num + i ) = (char)((sum % 10) + CHAR_CONV);	
			--num2_size;
			--i;
	}
	*(new_num + i ) = (char)((sum / 10) + CHAR_CONV);	/*add last number*/
    
	return new_num;	
}

void LargeNumbersTest()
{
	char string1[] = "999";
	char string2[] = "1";
	char *string12 = NULL;

	char string3[] = "999";
	char string4[] = "999";
	char *string34 = NULL;

	char string5[] = "5";
	char string6[] = "998";
	char *string56 = NULL;

	string12 = LargeNumbers(string1, string2);
	string34 = LargeNumbers(string3, string4);
	string56 = LargeNumbers(string5, string6);

	printf("LargeNumbersTest 1:	(result) %s == 1000 (desired)\n", string12);
	printf("LargeNumbersTest 2:	(result) %s == 1998 (desired)\n", string34);
	printf("LargeNumbersTest 3:	(result) %s == 1003 (desired)\n", string56);
	
	free(string12); /*free memmory allocated in LastNumbers() */
	free(string34);
	free(string56);
}

int IsPalindrome(const char *str)
{
	int i=0, k=0, j=0, str_len=0;
	for (;'\0' != str[str_len]; ++str_len);
	k = str_len-1;	
	for(; j < str_len/2; ++j)
	{
		if(str[i] != str[k])
		{
			return 0;
		}		
		++i; --k;
	}
	return 1;
}

void IsPalindromeTest()
{
	char string1[] = {"abckcba"};
	char string2[] = {"aaabaaa"};
	char string3[] = {"bgffgb"};

	printf("IsPalindromeTest 1:	%s\n", IsPalindrome(string1) ? "Passed" : "Failed");
	printf("IsPalindromeTest 2:	%s\n", IsPalindrome(string2) ? "Passed" : "Failed");
	printf("IsPalindromeTest 3:	%s\n", IsPalindrome(string3) ? "Passed" : "Failed");
}

int SevenBoom(int num)
{
	int seven_digit = num;	
	if (0 == num%7)
	{
		printf("BOOM");
		return 0;
	}
	else if (0 != num%7)
	{	
		while (0 != seven_digit )
		{
			if(7 == seven_digit%10)
			{
				printf("BOOM");
				return 0;
			}
			seven_digit /= 10;
		}
	}
	printf("%d",num);
	return 0;
}

void SevenBoomTest()
{
	printf("SevenBoomTest 1:	15 == ");	
	SevenBoom(15);
	printf("\nSevenBoomTest 2:	BOOM == ");	
	SevenBoom(21);
	printf("\nSevenBoomTest 3:	BOOM == ");	
	SevenBoom(177);
	printf("\nSevenBoomTest 4:	16 == ");	
	SevenBoom(16);	
	printf("\n");	
}
 
void swap(int **a, int **b) 
{
   int* tmp = *a;
    *a = *b;
    *b = tmp;
}

void swapTest()
{
	int num1 = 5, num2 = 7;
	int *ptr1 = &num1, *ptr2 = &num2;

	printf("swapTest - Before swap:	ptr1= %p ptr2= %p\n", (void *)ptr1, (void *)ptr2);
	swap(&ptr1, &ptr2);
	printf(" 	   After swap:	ptr1= %p ptr2= %p\n", (void *)ptr1, (void *)ptr2);
}

void RmSpaces(char *str)
{
	char *dest = str;  /* Destination to copy to */
	while (*str == ' ' || *str == '	') /* Ignore first spaces or tabs*/
	{
		++str;
	}
	/* While we're not at the end of the string, loop... */
	while (*str)
	{
		if(*str == '	') /*if the current character is tab, replace it with space*/
		{
			*str = ' ';
		}
	      	/* Loop while the current character is a space, AND the next
       	         * character is a space
        	 */
       		 while (*str == ' ' && *(str + 1) == ' ')
		 {
       	         	++str;  /* Just skip to next character */
		 }
       		 /* Copy from the "source" string to the "destination" string,
        	  * while advancing to the next character in both
       		  */
       		 *dest = *str;
			  ++dest;
			  ++str;
	}

        /* Make sure the string is properly terminated */    
		
	if(' ' == *dest) /*make sure no spaces at the end of string*/
	{         
		--dest;		
		*dest = '\0';
	}
	else
	{
		*dest = '\0';
	}
	str = dest;
}

void RmSpacesTest()
{
	char string_to_test[] = "   a  spacy    string   this one	indeed   ";
	char expected_result[] = "a spacy string this one indeed ";
	RmSpaces(string_to_test);
	printf("RmSpacesTest:		%s\n", !strcmp(string_to_test, expected_result) ? "Passed" : "Failed");
}
