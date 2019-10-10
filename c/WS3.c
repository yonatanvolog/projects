/********************************************************************
 *	This code is representing my functions of WS3					*
 *	Lab: OL-79 														*
 *	Author: Yonatan Vologdin 										*
 *	Reviewer: Erez Barr												*
 *	Date: 09.10.19													*
 ********************************************************************/
#include <stdio.h>		/*printf */
#include <stdlib.h>		/*calloc, malloc and free */
#include <string.h>		/*strlen */
#include <strings.h>	/*strcasecmp */
#include <assert.h>		/*assert */
#include <ctype.h>		/*tolower */

/*This function copies env varibles to a new variable and prints it out
 */
char** CopyEnv(char *env[]);

/*	This function copies cpy_string to des_string, lowers
 *	the letter if it's UPPER case and returns a pointer to des_string.
 */
char *StrCpyLower(char *des_string, const char *cpy_string);

/*	This function frees memmory allocated to env_to_print
 */
int FreeEnv(char *copied_env[]);
void CopyEnvTest(int argc, char *argv[], char *env[]);

/* Returns the id of the killed soldier, or negative id of the last man standing
 */
int KillOne(int current_id, int *soldier, int num_of_soldiers);

/* Returns the id of the soldier currently holding the sword,
 * or negative id of the last man standing
 */
int HandTo(int current_id, int *soldier, int num_of_soldiers);

/* This function uses KillOne() and HandTo() to kill all soldiers
 * till one last left, then returns his ID as int
 */
int BeginMassacre(int current_id, int *soldier, int num_of_soldiers);
void BeginMassacreTest();

/*	Prints out all data types in C and their sizes
 */
void DataTypes();

/*global variable so not run count loop twice (once in each loop)*/
int env_var_count_g = 0; 
 
int main(int argc, char *argv[], char *env[])
{	
	CopyEnvTest(argc, argv, env);
	BeginMassacreTest();
	DataTypes();

	return 0;
}

char** CopyEnv(char *env[])
{
	int env_string_len = 0, i = 0;	
	
	char **new_env = NULL;
	
	char **env_runner = env; 

	while(*env_runner)
	{
		++env_runner;
	}
	env_var_count_g = env_runner - env;
	new_env = (char **)calloc(env_var_count_g + 1, sizeof(char *));
	*(new_env + env_var_count_g) = '\0';

	for(; i < env_var_count_g; ++i)
	{
		env_string_len = strlen(*(env + i));
		*(new_env + i) = (char *)calloc(env_string_len + 1, sizeof(char));
		StrCpyLower(*(new_env + i), *(env + i));
	}	

	return new_env;
}

char *StrCpyLower(char *des_string, const char *cpy_string)
{
	char *des_start = des_string;

	assert(*cpy_string);
	
	/*while cpy_string character is not NULL, copy cpy to des and increment both*/	
	for(; '\0' != *cpy_string; 	cpy_string++, des_string++)
	{	
			*des_string = tolower(*cpy_string);
	}
	*des_string = '\0';
	
	return  des_start;
}

int FreeEnv(char *copied_env[])
{	
	char **free_env_parray = copied_env;

	while(*copied_env)
	{
		free(*copied_env);
		++copied_env;
	}
	free(free_env_parray);
	
	return 1;
}

void CopyEnvTest(int argc, char *argv[], char *env[])
{	
	char **env_to_print = NULL, **env_to_free = NULL;
	int is_free_suc = 0, count = 0, env_cmp = 0;

	env_to_print = CopyEnv(env);
	env_to_free = env_to_print;
	env_cmp = strcasecmp(*env, *env_to_print);

	while(0 == env_cmp && *env_to_print && *env)
	{
		env_cmp = strcasecmp(*env, *env_to_print);
		++env_to_print;
		++env;
		++count;
	}
	is_free_suc = FreeEnv(env_to_free);

	if(env_var_count_g == count && is_free_suc) 
	{
		printf("\nCopyEnvTest		   Passed!\n\n");
	}
	else
	{
		printf("\nCopyEnvTest	   	   Failed\n\n");
	}
}


int KillOne(int current_id, int *soldier, int num_of_soldiers)
{
	int alive_counter = 0, to_kill =0;
	
	while(alive_counter < num_of_soldiers) /*if FALSE - iterated through all soldiers*/
	{
		to_kill = (current_id + 1 + alive_counter) % num_of_soldiers;

		if(soldier[to_kill] && to_kill != current_id ) /*if new soldier is alive and it's not the current one*/
		{	
			soldier[to_kill] = 0; /*kills next alive soldier*/

			return to_kill;
		}
		++alive_counter;
		++to_kill;
	}
	
	return ((-1)*to_kill);
}

int HandTo(int current_id, int *soldier, int num_of_soldiers)
{
	int alive_counter = 0, hand_to = 0;
	
	while(alive_counter < num_of_soldiers) /*if FALSE - iterated through all soldiers*/
	{
		hand_to = (current_id + 1 + alive_counter) % num_of_soldiers;

		if(soldier[hand_to] && hand_to != current_id) /*if new soldier is alive and it's not the current one*/
		{
			return hand_to;
		}
		++alive_counter;
		++hand_to;
	}

	return ((-1)*hand_to);
}

int BeginMassacre(int current_id, int *soldier, int num_of_soldiers)
{
	while(1) 
	{
		current_id = KillOne(current_id, soldier, num_of_soldiers);

		if(0 > current_id)
		{
			return (-1)*current_id;
		}
		current_id = HandTo(current_id, soldier, num_of_soldiers);	
	}

	return 0; /*so gd would'nt scream at me*/
}

void BeginMassacreTest()
{
	
	int num_of_soldiers = 1000;   	/****ENTER HERE NUMBER OF SOLDIERS****/
	int starting_soldier_id = 0;    /****ENTER HERE FIRST SOLDIER POSITION****/
	int *soldier = (int *)malloc(sizeof(int)*num_of_soldiers); /* allocating space for soldiers array*/
	
	int i = 0, survivor = 0;
	
	for (; i < num_of_soldiers; ++i)	/*set all soldiers to be alive (binary 1)*/
	{
		soldier[i] = 1;
	}	
	survivor = BeginMassacre(starting_soldier_id, soldier, num_of_soldiers);
	
	if (977 == survivor)
	{	
		printf("BeginMassacreTest	   Passed!\n\n");
	}
	else
	{
		printf("BeginMassacreTest	   Failed!\n\n");
	}
	free(soldier);
}

void DataTypes()
{
	printf("sizeof(signed char):		%ld\n",sizeof(signed char));
	printf("sizeof(unsigned char):		%ld\n",sizeof(unsigned char));	
	printf("sizeof(short int):		%ld\n",sizeof(short int));
	printf("sizeof(unsigned short int):	%ld\n",sizeof(unsigned short int));
	printf("sizeof(unsigned int):		%ld\n",sizeof(unsigned int));
	printf("sizeof(int):			%ld\n",sizeof(int));
	printf("sizeof(float):			%ld\n",sizeof(float));
	printf("sizeof(long int):		%ld\n",sizeof(long int));
	printf("sizeof(unsigned long int):	%ld\n",sizeof(unsigned long int));
	/*printf("sizeof(long long int):		%ld\n",sizeof(long long int));             --NOT SUPPORTED*/
	/*printf("sizeof(unsigned long long int):	%ld\n",sizeof(unsigned long long int));--NOT SUPPORTED*/
	printf("sizeof(double):			%ld\n",sizeof(double));
	printf("sizeof(long double):		%ld\n\n",sizeof(long double));
}
