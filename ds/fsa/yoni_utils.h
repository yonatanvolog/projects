/**********************************************************************
 *	This code is representing my utility macros 		      	 	  *
 *	Lab: OL-79                                		                  *
 *	Author: Yonatan Vologdin 										  *	
 **********************************************************************/

/*Macro to eliminate compiler warnings of unused variables*/
#define	UNUSE(var) ((void)(var))

/*Macro to run tests in main*/
#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define RUN_SEQUENCE(test_name, test)       \
printf("\n******* %s *******\n\n",test_name);\
test								          \

/*Macro to run tests in test function*/
#define RUN_TEST(test_name, result) \
(printf("%s %s\n", test_name, (1 == (result)) ? KGRN"\tPassed"KNRM : KRED"\tFailed"KNRM))

/*Macro which returns max value out of 2 values*/
#define MAX(val1, val2) (((val1) > (val2)) ? (val1) : (val2))

#define WORD_SIZE (sizeof(size_t))
