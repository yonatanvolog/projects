/**********************************************************************
 *	This code is representing my APP for WD RC1                       *
 *	Lab: OL-79                                		                  *
 *	Author: Yonatan Vologdin 										  *
 *	Reviewer: Shaddad Gomid  	  								      *
 *	Date: 20.01.20													  *
 **********************************************************************/
#include <stdio.h>  /*printf*/
#include <signal.h> /* sigaction*/
#include <unistd.h> /*sleep*/

#include "watcher.h"
/*
#define TEST_DNR
*/
/*
#define TEST_APP_CRASH
*/
#define DEAD_TIME (5)
#define INTERVAL (1)
#define UNUSE(x) (void)(x)

int MMI(char **argv, int dead_time, int interval);

int main(int argc, char **argv)
{   
#ifdef TEST_DNR
    int i = 0;
#endif

#ifdef TEST_APP_CRASH
    int i = 0;
#endif

    int k = 0;
    int dead_time = DEAD_TIME;
    int interval = INTERVAL;

    UNUSE(argc);

    if (0 != MMI(argv, dead_time, interval))
    {
        printf("\n\nMMI failed\n\n");
        
        return 1;
    }

    for(k = 0; k < 30; ++k)
    {
        printf("executing critial section\n");

#ifdef TEST_DNR
        printf("\t\t\tTHREAD: waiting for DNR to activate in %d/10\n", i);
        if (i == 10)
        {
            DNR();
        }
        ++i;
#endif

#ifdef TEST_APP_CRASH
        printf("THREAD: Killing himself in %d/10 (TEST_APP_CRASH)\n", i);
        if (i == 10)
        {
            sleep(1);
	        kill(getpid(), SIGKILL);
	        printf("\t\t\tTHREAD: THREAD shoud be dead!\n");
        }
        ++i;
#endif

        sleep(1);
    }
    printf("\nAPP: APP terminated successfully!\n");

    return 0;
}