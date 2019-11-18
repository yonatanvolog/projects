/**********************************************************************
 *	This code is representing my functions of Unique ID RC2           *
 *	Lab: OL-79                                		                  *
 *	Author: Yonatan Vologdin 										  *
 *	Reviewer: Dvir Breitbart 									      *
 *	Date: 18.11.19													  *
 **********************************************************************/
#include <stdlib.h>		/*malloc*/
#include <unistd.h>		/*get pid*/

#include "uid.h"		/*unique ID header*/

const ilrd_uid_t BAD_UID = {0, -1, 0};

ilrd_uid_t UIDCreate(void)
{	
	static size_t counter = 0;
	ilrd_uid_t new_uid = {0, 0, 0};

	new_uid.pid = getpid();
	new_uid.time = time(NULL);
	new_uid.counter = counter;
	++counter;

	return new_uid;
}

int UIDIsSame(ilrd_uid_t uid1, ilrd_uid_t uid2)
{
	if (UIDIsBad(uid1) || UIDIsBad(uid2))
	{
		return -1;
	}

	return	((uid1.pid == uid2.pid) && 
			 (uid1.time == uid2.time) && 
			 (uid1.counter == uid2.counter));
}

int UIDIsBad(ilrd_uid_t uid)
{
	return (BAD_UID.time == uid.time);
}
