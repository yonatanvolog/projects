/**********************************************************************
 *	This code is representing my tests of template     	              *
 *	Lab: OL-79                                		                  *
 *	Author: Yonatan Vologdin 										  *
 *	Date: 						        							  *
 **********************************************************************/
#include <stdio.h>	    /*printf*/

#include "yoni_utils.h" /*RUN_SEQUENCE, RUN_TEST*/

int Test1();

int main()
{
	RUN_SEQUENCE("Test1", Test1());

	return 0;
}

int Test1()
{
	RUN_TEST("Test1", 1 == 1);
	
	return 0;
}

