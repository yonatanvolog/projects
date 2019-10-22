#include <stdio.h>	/*printf*/
#include <string.h>	/*strlen*/

/*	This function recieves 2 strings, returns 1 if string2 is rotation of string1.
 *	Otherwise returns 0.
 */
int IsRotate(char *string1, char *string2);

int main()
{
	char string1[] = {1, 2 ,1 ,1, 2, 1, '\0'};
	char string2[] = {2, 1 ,1 ,2, 1, 2, '\0'};
/*
	char string1[] = {1, 2 ,3 ,1, 5, 6, '\0'};
	char string2[] = {2, 3 ,1 ,5, 6, 1, '\0'};
	
	char string1[] = {1, 2 ,3 ,1, 5, 6, '\0'};
	char string2[] = {2, 3 ,1 ,5, 6, 1, '\0'};
*/
	if (IsRotate(string1, string2))
	{
		printf("Strings are same, but rotated\n");
	}
	else
	{
		printf("Strings are different\n");
	}
	
	return 0;
}

int IsRotate(char *string1, char *string2)
{
	char *runner1 = string1, *runner2 = string2;
	int index1 = 0, index2 = 0, count = 0;
	
	int string1_len = strlen(string1)-1;
	int string2_len = strlen(string2)-1;
	
	if(string1_len != string2_len)
	{
		return 0;
	}
	
	while(string1_len - (runner1 - string1) > 0)
	{	
		while (*runner1 != *runner2)
		{
			++runner2;
		}
		
		while (runner1[index1] == runner2[index2])
		{
			index1 = (index1 + 1) % string1_len;
			index2 = (index2 + 1) % string2_len;			
			++count;
			if(string1_len == count)
			{
				return 1;
			}
		}
		runner2 = string2;
		count = 0;
		index1 = 0;
		index2 = 0;
		++runner1;
	}
	
	return 0;
}
