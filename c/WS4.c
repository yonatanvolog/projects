/********************************************************************
 *	This code is representing my functions of WS4					*
 *	Lab: OL-79 														*
 *	Author: Yonatan Vologdin 										*
 *	Reviewer: Eyal Rabinovitz										*
 *	Date: 10.10.19													*
 ********************************************************************/

#include <stdio.h>	/*printf*/
#include <stdlib.h>	/*getchar*/

#define ESC 27
#define FN NullFunc
#define ESC 27

void PrintA();
void Printa();
void PrintT();
void Printt();
void PrintESC();
void NullFunc();

void LUTtest();
void IfElseTest();
void CaseTest();

void (*lookup[256])(void) =  {FN,FN,FN,FN,FN,FN,FN,FN,FN,FN,			/*0-9*/
				  			  FN,FN,FN,FN,FN,FN,FN,FN,FN,FN,			/*10-19*/
			  		  		  FN,FN,FN,FN,FN,FN,FN,PrintESC,FN,FN,		/*20-29*/
							  FN,FN,FN,FN,FN,FN,FN,FN,FN,FN,			/*30-39*/
							  FN,FN,FN,FN,FN,FN,FN,FN,FN,FN,			/*40-49*/
							  FN,FN,FN,FN,FN,FN,FN,FN,FN,FN,			/*50-59*/
							  FN,FN,FN,FN,FN,PrintA,FN,FN,FN,FN, 		/*60-69*/	
							  FN,FN,FN,FN,FN,FN,FN,FN,FN,FN,			/*70-79*/
							  FN,FN,FN,FN,PrintT,FN,FN,FN,FN,FN,  		/*80-89*/
							  FN,FN,FN,FN,FN,FN,FN,Printa,FN,FN,		/*90-99*/
							  FN,FN,FN,FN,FN,FN,FN,FN,FN,FN,			/*100-109*/
							  FN,FN,FN,FN,FN,FN,Printt,FN,FN,FN,		/*110-119*/
							  FN,FN,FN,FN,FN,FN,FN,FN,FN,FN,			/*120-129*/
							  FN,FN,FN,FN,FN,FN,FN,FN,FN,FN,			/*130-139*/
							  FN,FN,FN,FN,FN,FN,FN,FN,FN,FN,			/*140-149*/
							  FN,FN,FN,FN,FN,FN,FN,FN,FN,FN,			/*150-159*/
							  FN,FN,FN,FN,FN,FN,FN,FN,FN,FN,			/*160-169*/
							  FN,FN,FN,FN,FN,FN,FN,FN,FN,FN,			/*170-179*/
							  FN,FN,FN,FN,FN,FN,FN,FN,FN,FN,			/*180-189*/
							  FN,FN,FN,FN,FN,FN,FN,FN,FN,FN,			/*190-199*/
							  FN,FN,FN,FN,FN,FN,FN,FN,FN,FN,			/*200-209*/
							  FN,FN,FN,FN,FN,FN,FN,FN,FN,FN,			/*210-219*/
							  FN,FN,FN,FN,FN,FN,FN,FN,FN,FN,			/*220-229*/
							  FN,FN,FN,FN,FN,FN,FN,FN,FN,FN,			/*230-239*/
							  FN,FN,FN,FN,FN,FN,FN,FN,FN,FN,			/*240-249*/
							  FN,FN,FN,FN,FN,FN};					/*250-255*/		
				 			 
int main() /*Make sure to leave only one function NOT commented out*/
{
    LUTtest();
  	/*IfElseTest();*/
  	/*CaseTest();*/
    
    return 0;
}

void PrintA()
{
	printf("pressed A\n");
}
void Printa()
{
	printf("pressed a\n");
}
void PrintT()
{
	printf("pressed T\n");
}
void Printt()
{
	printf("pressed t\n");
}
void PrintESC()
{
    system("stty icanon echo");
    
	exit(0);
}
void NullFunc(){;}

void LUTtest()
{
	char input = '\0';
	
	system("stty -icanon -echo");
	
	printf("Enter A or T, press ESC to quit: ");
	
    while(1)
    {
    	input = getchar();
		(*lookup[(unsigned int)input])();
    }
}

void IfElseTest()
{
	char input = '\0';
	system("stty -icanon -echo");
	printf("Enter A or T, press ESC to quit: ");
	
    while(1)
    {
    	input = getchar();
    	
		if ('a' == input || 'A' == input)
		{
			printf("A pressed\n");
		}
	
		else if ('t' == input || 'T' == input)
		{
			printf("T pressed\n");
		}
		
		else if(27 == input)
		{
			exit(0);
		}
		
		else
		{
			;
		}
	}
	system("stty icanon echo");
}

void CaseTest()
{
    char input = '\0';
	system("stty -icanon -echo");
	printf("Enter A or T, press ESC to quit: ");

    while(1)
    {
    	input = getchar();
    	
    	switch(input)
    	{
    	   	 case 'a':
				printf("a pressed\n");
				break;
				
			 case 'A':
				printf("A pressed\n");
				break;	  
				        
       	 	 case 't':
				printf("t pressed\n");
				break;
			
			 case 'T':
				printf("T pressed\n");
				break;  
			
			case 27:
				exit(0);

	        default:
				break;
		}
	}
	system("stty icanon echo");
}
