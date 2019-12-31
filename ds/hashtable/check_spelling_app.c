/**********************************************************************
 *	This code is representing my check spelling app RC2	              *
 *	Lab: OL-79                                		                  *
 *	Author: Yonatan Vologdin 										  *
 *	Reviewer: Dvir Breitbart 	 	  						     	  *
 *	Date: 29.12.19													  *
 **********************************************************************/
 
#include <stdio.h>      /*printf*/
#include <string.h>     /*strtok*/
#include <assert.h>     /*assert*/ 	
#include <sys/mman.h>   /*mmap*/
#include <sys/stat.h>   /*open*/
#include <unistd.h>     /*lseek*/
#include <fcntl.h>      /*O_RDONLY*/
#include <ctype.h>      /*toupper*/
#include <strings.h>    /*strcasecmp*/

#include "hashtable.h"

#define NUM_OF_LETTERS_IN_ABC (1 + 'z' - 'a')
#define USER_STRING_BUFFER_SIZE 1000
#define A_ASCII ('A')
#define X_ASCII ('X'-'A')
#define Z_ASCII ('Z'-'A')

static int mmapDictionaryIMP(hash_table_t *hash_table);
static size_t GetFileSizeIMP(int file_descriptor);
static void CheckSpellingIMP(const hash_table_t *hash_table, char *user_input);
static size_t HashFunc(const void *key);
static int IsMatchFunc(const void *current_data, const void *data_to_compare);

int main()
{	
	size_t num_of_buckets = NUM_OF_LETTERS_IN_ABC;
	char user_input[USER_STRING_BUFFER_SIZE] = {0};
	int return_value = 0;
	hash_table_t *hashtable = HashCreate(num_of_buckets,
                         				 HashFunc,
                         				 IsMatchFunc);
	                    				
    return_value = mmapDictionaryIMP(hashtable);
    assert(0 == return_value);
    
	printf("Enter a string:\n");
	scanf("%[^\n]s", user_input);
	printf("Below words are misspelled:\n");
    CheckSpellingIMP(hashtable, user_input);
	
    HashDestroy(hashtable);
    
    return 0;
}

static int mmapDictionaryIMP(hash_table_t *hash_table)
{
	char *token = NULL;
	char *dictionary = NULL;
	int fd = open("/usr/share/dict/words", O_RDONLY);
	size_t file_size = GetFileSizeIMP(fd);
	
    dictionary = mmap(NULL, file_size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
	
	token = strtok(dictionary, ",. !\n");
	while (NULL != token)
	{
		if (1 == HashInsert(hash_table, token))
		{
		    return 1;
		}
		token = strtok(NULL, ",. !\n");
	}

	return 0;
}

static size_t GetFileSizeIMP(int fd)
{
	size_t size = lseek(fd, 0, SEEK_END);   /*seek to end of file*/
	lseek(fd, 0, SEEK_SET);                 /*return to beginning of file*/
	
	return size;
	
}

void CheckSpellingIMP(const hash_table_t *hash_table, char *user_input)
{
	char *token;
	token = strtok(user_input, ",. !");
	while (token != NULL)
	{
		if (NULL == HashFind(hash_table, token))
		{
			printf("%s, ", token);
		}
		token = strtok(NULL, ",. !");
	}
	printf("\n");
}

static size_t HashFunc(const void *key)
{
	size_t index = (size_t)toupper(*(char *)key) - 'A';
	if (index < A_ASCII || index > Z_ASCII)
	{
		index = X_ASCII;                 /*put weird words to 'X' bucket*/
	}
	
	return index;
}

static int IsMatchFunc(const void *current_data, const void *data_to_compare)
{
	return (0 == strcasecmp((const char *)current_data, 
	                        (const char *)data_to_compare));
}
