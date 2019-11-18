/**********************************************************************
 *	This code is representing my tests of Unique ID RC2        		  *
 *	Lab: OL-79                                		                  *
 *	Author: Yonatan Vologdin 										  *
 *	Reviewer: Dvir Breitbart    	  								  *
 *	Date: 18.11.19													  *
 **********************************************************************/
#include <stdio.h>		/*printf*/
#include <sys/types.h>	/*pid_t type*/
#include <unistd.h>		/*sleep*/

#include "uid.h"	/*unique ID header*/

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"

#define RUN_SEQUENCE(test_name, test)       \
printf("\n******* %s *******\n\n",test_name);\
test								          \

#define RUN_TEST(test_name, result) \
(printf("%s %s\n", test_name, (1 == (result)) ? KGRN"\tPassed"KNRM : KRED"\tFailed"KNRM))

int UIDCreateTest();
int UIDIsSameTest();
int UIDIsBadTest();

int main()
{
	RUN_SEQUENCE("UIDCreateTest", UIDCreateTest());
	RUN_SEQUENCE("UIDIsSameTest", UIDIsSameTest());
	RUN_SEQUENCE("UIDIsBadTest", UIDIsBadTest());

	return 0;
}

int UIDCreateTest()
{
	ilrd_uid_t uid1 = {0};
	ilrd_uid_t uid2 = {0};
	ilrd_uid_t uid3 = {0};

	uid1 = UIDCreate();
	printf("uid1.pid: %d\n", uid1.pid);
	printf("uid1.time: %ld\n", uid1.time);	
	printf("uid1.counter: %ld\n\n", uid1.counter);

	printf("***sleeping 3 seconds***\n");
	sleep(3);
	uid2 = UIDCreate();
	printf("uid2.pid: %d\n", uid2.pid);
	printf("uid2.time: %ld\n", uid2.time);	
	printf("uid2.counter: %ld\n\n", uid2.counter);

	uid3 = UIDCreate();
	printf("uid3.pid: %d\n", uid3.pid);
	printf("uid3.time: %ld\n", uid3.time);	
	printf("uid3.counter: %ld\n", uid3.counter);

	return 0;	
}

int UIDIsSameTest()
{
	ilrd_uid_t uid1 = UIDCreate();
	ilrd_uid_t uid2 = UIDCreate();

	RUN_TEST("IsSameTest1", 0 == UIDIsSame(uid1, uid2));

	uid2 = uid1;
	RUN_TEST("IsSameTest2", 1 == UIDIsSame(uid1, uid2));

	RUN_TEST("IsSameTest3", -1 == UIDIsSame(uid1, BAD_UID));

	return 0;	
}

int UIDIsBadTest()
{
	ilrd_uid_t uid = UIDCreate();

	RUN_TEST("IsBadTest1", 0 == UIDIsBad(uid));
	RUN_TEST("IsBadTest2", 1 == UIDIsBad(BAD_UID));

	return 0;	
}
