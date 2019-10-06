/********************************************************************
 *	This is code repreenting my implementation of string.h library	*
 *	Lab: OL-79 														*
 *	Author: Yonatan Vologdin 										*
 *	Date: 02.10.19													*
 ********************************************************************/

#include <stdio.h>	/* for printf() */
#include <assert.h> /* for assert() function */
#include <string.h> /* for original string.h funcs for comparioson*/
#include <stdlib.h> /* for malloc() */

/* This function returns the number of charcters 
 * (excluding NULL) in string.
 */
size_t MyStrLen(const char *string);
void MyStrLenTest();

/*	This function compares string1 with string2.
 *	Returns 0 if strings are equal or the ASCII
 *	value difference between unmatching characters.
 */	
int MyStrCmp(const char *string1, const char *string2);
void MyStrCmpTest();

/*	This function copies cpy_string to des_string
 *	and returns a pointer to des_string.
 */
char *MyStrCpy(char *des_string, const char *cpy_string);
void MyStrCpyTest();

/*	This function copies at most n characters from 
 *	cpy_string to des_string. If cpy_des is less than n, the function
 *  writes additional NULL bytes till 'n' bytes are written. 
 *	Returns a pointer to des_string.
 */
char *MyStrnCpy(char *cpy_string, char *des_string, size_t n);
void MyStrnCpyTest();

/*	This function compares string1 and string2 without case sensitivity.
 *	Internally it converts all letters to lower case.
 *	Returns 0 if strtings are equal or the difference between unmatching
 *	characters in ASCII value. 
 */
int MyStrCaseCmp(char *string1, char *string2);
void MyStrCaseCmpTest();


/*	This function returns a pointer to the first occurance of char_in_int int string. 
 *	The NULL chracter is also condsidered part of 'string' and can be located as well.
 */
char *MyStrChr(const char string[], int char_in_int);
void MyStrChrTest();

/*	This function returns a pointer to a duplicate string of "str".
 *	Returns NULL if str is NULL or allocation error occured.
 */
char *MyStrDup(const char *str);
void MyStrDupTest();

/*	This function appends a copy of scr string to the end of dest, overwrites the 
 *	terminating NULL character and adds a new one at the end of the new string.
 *	Returns a pointer to dest.
 */
char *MyStrCat(char *dest, const char *scr);
void MyStrCatTest();

/*	This function appends the first num characters of source to destination,
 *	plus a terminating null-character.
 *	If the length of the C string in source is less than num, 
 *	only the content up to the terminating null-character is copied.
 */
char *MyStrnCat(char *dest, const char *scr, size_t num);
void MyStrnCatTest();

void main()
{
	MyStrLenTest();
	MyStrCmpTest();
	MyStrCpyTest();
	MyStrnCpyTest();
	MyStrCaseCmpTest();
	MyStrChrTest();
	MyStrCatTest();
	MyStrnCatTest();
}

size_t MyStrLen(const char *string)
{
	long i = 0;
	assert(*string); 					/*check string is not NULL*/
	for (; '\0' != *(string + i); ++i); /*increments i while character in not NULL*/
	return i;
}

void MyStrLenTest()
{
	/*printf("%s\n", MyStrLen("") == strlen("") ? "Passed" : "Failed");	assertion fail 
	printf("%s\n", MyStrLen("\0") == strlen("\0") ? "Passed" : "Failed");	assertion fail */
	printf("MyStrLenTest a:	%s\n", MyStrLen("a") == strlen("a") ? "Passed" : "Failed");	
	printf("MyStrLenTest abc:	%s\n", MyStrLen("abc") == strlen("abc") ? "Passed" : "Failed");	
}


int MyStrCmp(const char *string1, const char *string2)
{				
	assert(*string1 && *string2); /* check both strings are not NULL*/
	while ('\0' != *string1 && '\0' != *string2)
	{	
		if (*string1 != *string2)
		{	
			return *string1 - *string2;
		}
		*string1++;
		*string2++;
	}
	return *string1 - *string2;
}

void MyStrCmpTest()
{
	/*printf("%s\n", MyStrCmp("","") == strcmp("","") ? "Passed" : "Failed");	assertion fail*/
	/*printf("%s\n", MyStrCmp("\0","\0") == strcmp("\0","\0") ? "Passed" : "Failed");	assertion fail*/
	printf("MyStrCmpTest hi hi:	%s\n", MyStrCmp("hi","hi") == strcmp("hi","hi") ? "Passed" : "Failed");	
	printf("MyStrCmpTest hia hia:	%s\n", MyStrCmp("hia","hia") == strcmp("hia","hia") ? "Passed" : "Failed");
	printf("MyStrCmpTest Hi Hi:	%s\n", MyStrCmp("Hi","Hi") == strcmp("Hi","Hi") ? "Passed" : "Failed");
}

char *MyStrCpy(char *des_string, const char *cpy_string)
{
	char *des_start = des_string; 
	assert(*cpy_string);		
	/*while cpy_string character is not NULL, copy cpy to des and increment both*/	
	for(; '\0' != *cpy_string; 	*cpy_string++, *des_string++)
	{	
		*des_string = *cpy_string;
	}
	*des_string = '\0';  /*places last character (/0) in des_string */
	return  des_start; 			/*returns pointer to first character of des_string*/
}

void MyStrCpyTest()
{
		char string[4];
	 	char copy[] = "Hi";	
		
		printf("MyStrCpyTest copy->string:	%s\n", MyStrCpy(copy ,string) == strcpy(copy ,string) ? "Passed" : "Failed");	
}

char *MyStrnCpy(char *des_string, char *cpy_string, size_t n)
{
	assert(*cpy_string);
	long i = 0;
	for (; i < n && '\0' != *(cpy_string + i); ++i)
	{
		*(des_string + i) = *(cpy_string + i);
	}
	for (; i < n; ++i)
	{
		*(des_string + i) = '\0';
	}
	return  des_string;
}

void MyStrnCpyTest()
{
		char string[7];
	 	char copy[] = "Howdie partner";	
		
		printf("MyStrnCpyTest copy4->string:	%s\n", MyStrnCpy(copy ,string,4) == strncpy(copy ,string,4) ? "Passed" : "Failed");
		printf("MyStrnCpyTest copy10->string:	%s\n", MyStrnCpy(copy ,string,10) == strncpy(copy ,string,10) ? "Passed" : "Failed");		
}

int MyStrCaseCmp(char *string1, char *string2)
{
	int i = 0;
	while ('\0' != string1[i]  && '\0' != string2[i])
	{
		if ((int)string1[i] > 65 && (int)string1[i] < 90)
		{
			string1[i] += 32; /*converting UPPER case to lower case*/
		}
		if ((int)string2[i] > 65 && (int)string2[i] < 90)
		{
			string2[i] += 32; /*converting UPPER case to lower case*/
		}
		if (string1[i] != string2[i])
		{
			return string1[i] - string2[i];
		}
	++i;
	}
	return string1[i] - string2[i];
}

void MyStrCaseCmpTest()
{
		char string1lower[] = "String number one";
	 	char string1UPPER[] = "STRING NUMBER ONE";
		char string1Mixed[] = "StRing NUMBer onE";
		
		printf("MyStrCaseCmpTest lower UPPER:	%s\n", MyStrCaseCmp(string1lower ,string1UPPER) == strcasecmp(string1lower ,string1UPPER) ? "Passed" : "Failed");
		printf("MyStrCaseCmpTest lower MiXed:	%s\n", MyStrCaseCmp(string1lower ,string1Mixed) == strcasecmp(string1lower ,string1Mixed) ? "Passed" : "Failed");
		printf("MyStrCaseCmpTest UPPER MiXed:	%s\n", MyStrCaseCmp(string1UPPER ,string1Mixed) == strcasecmp(string1UPPER ,string1Mixed) ? "Passed" : "Failed");
}

char *MyStrChr(const char string[], int char_in_int)
{	
	char to_find = char_in_int;
	assert(*string); /*check if string is not NULL */	
	if('\0' != to_find)
	{	
		while('\0' != *string)
		{	
			if(to_find == *string)
			{				
				return (char *)(string);
			}
			*string++;
		}	
	}
	else /* '\0' == to_find */ 
	{
		for(; '\0' != *string; *string++);			
		return (char *)(string);
	}
	return NULL;
}

void MyStrChrTest()
{
	char string[] = {"nice string"};
	int n_letter  = 'n';
	int g_letter  = 'g';
	printf("MyStrChrTest n:	%s\n", MyStrChr(string ,n_letter) == strchr(string ,n_letter) ? "Passed" : "Failed");
	printf("MyStrChrTest g:	%s\n", MyStrChr(string ,g_letter) == strchr(string ,g_letter) ? "Passed" : "Failed");
}

char *MyStrDup(const char *str)
{
	int str_len = 0, i = 0;
	char *dup_str;
	/*no need for assert because MyStrDup handles NULL strings*/
	if (NULL == str)
	{
		return NULL;
	}	
	for(; '\0' != *(str + str_len); ++str_len);
	dup_str = malloc(str_len + 1);
	if (!dup_str) /*check if malloc allocation was successful, if not returns NULL*/
	{
		return NULL;
	}
	for(; i < str_len; ++i)
	{
		*(dup_str + i) = *(str +i);
	}	
	*(dup_str + i + 1) = '\0';
	return dup_str;
}

void MyStrDupTest()
{
	char string[] = {"a string to duplicate"};
	char My = *(MyStrDup(string));
	char orig = (char)(strdup(string));
	printf("MyStrDupTest:	%s\n", My == orig ? "Passed" : "Failed");
}

char *MyStrCat(char *dest, const char *scr)
{
	int i = 0, result_len = 0;
	assert(*dest && *scr);
	while (*(dest + i))
	{
		++i;
		++result_len;
	}
	i = 0;
	
	while (*(scr + i))
	{
		++i;
		++result_len;
	}
	*(dest + result_len);
	result_len = 0;
	i = 0;
	
	while (*(dest + i))
	{
		*(dest + result_len) = *(dest + i) ;
		++i;
		++result_len;
	} 
	i = 0;

	while (*(dest + i))
	{
		*(dest + result_len) = *(scr + i) ;
		++i;
		++result_len;
	}
	*(dest + result_len +1) = '\0';
	return dest; 
}

void MyStrCatTest()
{
	char string1[] = "This cat's on fire!";
	char string2[] = " Somebody help the poor kitty!";
	printf("MyStrCatTest:	%s\n", MyStrCat(string1 ,string2) == strcat(string1 ,string2) ? "Passed" : "Failed");
}

char *MyStrnCat(char *dest, const char *scr, size_t num)
{
	int i = 0, des_len = 0, scr_len = 0, result_len = 0;
	assert(*dest && *scr);
	while (*(dest + i))
	{
		++i;
		++des_len;
	}
	i = 0;
	while (*(scr + i))
	{
		++i;
		++scr_len;
	}
	if (scr_len <= num)		
	{	
		*(dest + des_len + scr_len);
		i = 0;
		while (*(dest + i))
		{
			*(dest + result_len) = *(dest + i);
			++i;
			++result_len;
		} 
		i = 0;
		while (*(dest + i))
		{
			*(dest + result_len) = *(scr + i);
			++i;
			++result_len;
		}
		*(dest + result_len + 1) = '\0';
	}	
	else /* scr_len > num */
	{ 	
		*(dest + des_len + num);
		i = 0;		
		while (*(dest + i))
		{
			*(dest + result_len) = *(dest + i);
			++i;
			++result_len;
		}
		i = 0; 
		while (i < num)
		{
			*(dest + result_len) = *(scr + i);
			++i;
			++result_len;
		}
		*(dest + result_len)= '\0';
	}
	return dest; 
}

void MyStrnCatTest()
{
	char string1[] = "This cat's on fire!";
	char string2[] = " Somebo/dy help the poor kitty!";
	printf("MyStrnCatTest:	%s\n", MyStrnCat(string1 ,string2, 4) == strncat(string1 ,string2, 4) ? "Passed" : "Failed");
}




	

