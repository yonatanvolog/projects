/******************************************************************************
*	The watchdog utility is a program designed to protect your program        *
*   from crashing.                                                            *
*   Should it crash, the watchdog is going to ressurect it.                   *
*                                                                             *
*   The way it's supposed to be used, is to place your "critical section"     *
*   code between the functions MMI and DNR in the following way:              *
*                                                                             *
*   int main(int argc, char **argv)                                           *
*   {                                                                         *
*       if (0 != MMI(argv, dead_time, interval))                              *
*       {                                                                     *
*           printf("MMI failed");                                             *
*                                                                             *
*           return 1;                                                         *
*       }                                                                     *
*                                                                             *
*       ************************                                              *
*       *** critical section ***                                              *     
*       ************************                                              *
*                                                                             *
*       DNR();                                                                *
*                                                                             *
*       return 0;                                                             *
*   }                                                                         *
*                                                                             *
*   Note that should your program fail, it would start from the beginning     *
*   and now from the point of filure.                                         *
*******************************************************************************/

/*MMI should recieved the argv parameter from main (just pass it through).
dead_time * interval will determine the total time your critical section 
may be down.
On success MMI returns 0, otherwise returns 1. */
int MMI(char **argv, int dead_time, int interval);

/*Decativates MMI */
void DNR(void);
