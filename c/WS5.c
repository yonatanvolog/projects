/**********************************************************************
 *	This code is representing my implementation of WS5 - Files        *
 *	Lab: OL-79                                                        *
 *	Author: Yonatan Vologdin                                          *
 *	Reviewer: Gal Salemon                                             *
 *	Date: 17.10.19                                                    *
 **********************************************************************/

#include <stdio.h>		/*printf*/
#include <string.h>		/*strcmp, strncmp*/
#include <stdlib.h>		/*exit*/
#include <assert.h>		/*assert*/

#define	MAX_LIMIT 		50
#define	TEMP_TEXT_FILE	temp_copy.txt

enum status_return_t {OPERATED, FAILED_TO_ACCESS_FILE, FAILED_TO_RENAME} status;

enum status_return_t RemoveOp(char *string, char **argv);
enum status_return_t CountOp(char *string, char **argv);
enum status_return_t ExitOp();
enum status_return_t AddFirstOp(char *string, char **argv);
enum status_return_t AddLastOp(char *string, char **argv);
int Cmp(char *keyword, char *input);
int CmpN(char *keyword, char *input);
int Empty();
/*auxilary function for CountOp*/
int countlines(char **argv);

int main(int argc, char *argv[])
{
	struct link_t
	{	
		char *keyword;
		int (*StrCmp)(char *keyword, char *input);
		enum status_return_t (*Operation)();
	};
	
	int i = 0, enum_res = 0;
	char input[MAX_LIMIT];

	struct link_t chain[] = {{"-remove\n",	Cmp,	RemoveOp},
	 						 {"-count\n",	Cmp,	CountOp},
							 {"-exit\n",	Cmp,	ExitOp},
	   						 {"<",			CmpN,	AddFirstOp},
	    					 {"",			Empty,	AddLastOp}};
	
	while (1)
	{
		printf("\nEnter string: ");
		fgets(input, MAX_LIMIT, stdin);

		for (i = 0; i < 5; ++i)
		{
			if (0 == chain[i].StrCmp(chain[i].keyword, input))
			{
				enum_res = chain[i].Operation(input, argv);
				printf("*** Return code: %d ***\n", enum_res);
		
				if (OPERATED == enum_res || FAILED_TO_ACCESS_FILE == enum_res)
				{
					break;
				}		
			}		
		}
	}

	return 0;
}

int countlines(char **argv)
{
	FILE *fp = fopen(argv[1],"rt");
	int ch = 0, lines = 0;

	assert(argv);

	while (!feof(fp))
	{
		ch = fgetc(fp);
		if (ch == '\n')
		{
			++lines;
		}
	}
	fclose(fp);
	
	return lines;
}

enum status_return_t RemoveOp(char *string, char **argv)
{
	FILE *fp = fopen(argv[1],"rt");

	assert(argv && string);

	if (NULL == fp)
	{
		return FAILED_TO_ACCESS_FILE;
	}
	fclose(fp);

	if (!remove(argv[1]))
	{
		printf("***File removed***\n");
	}

	return OPERATED;
}

enum status_return_t CountOp(char *string, char **argv)
{
	FILE *fp = fopen(argv[1],"rt");

	assert(argv && string);
	
	if (NULL == fp)
	{
		return FAILED_TO_ACCESS_FILE;
	}
	fclose(fp);
	printf("*** Num of lines: %d ***\n", countlines(argv));

	return OPERATED;
}

enum status_return_t ExitOp()
{
	printf("*** Exiting program ***\n");
	exit (0);
}

enum status_return_t AddFirstOp(char *string, char **argv)
{
	char ch;
	FILE *fp_temp;
	FILE *fp = fopen(argv[1],"rt");

	assert(argv && string);
	
	if (NULL == fp)
	{
		return FAILED_TO_ACCESS_FILE;
	}

	fp_temp = fopen("TEMP_TEXT_FILE","w");
	fputs((string + 1), fp_temp);
			
	while ((ch = fgetc(fp)) != EOF)
	{
		fputc(ch, fp_temp);
	}	
	remove(argv[1]);
	fclose(fp);
	if (0 != rename("TEMP_TEXT_FILE", argv[1]))
	{
			return FAILED_TO_RENAME;
	}			
	fclose(fp_temp);

	return OPERATED;
}

enum status_return_t AddLastOp(char *string, char **argv)
{
	FILE *fp = fopen(argv[1],"rt");

	assert(argv && string);

	if (NULL == fp)
	{
		return FAILED_TO_ACCESS_FILE;
	}
	fclose(fp);
	fp = fopen(argv[1],"at");
	fputs(string, fp);
	fclose(fp);
	printf("*** Inserted string to file ***\n");
	
	return OPERATED;
}

int Cmp(char *keyword, char *input)
{
	assert(keyword && input);

	return strcmp(keyword , input);
}

int CmpN(char *keyword, char *input)
{
	assert(keyword && input);

	return strncmp(keyword ,input, 1);
}

int Empty()
{
	return 0;
}


