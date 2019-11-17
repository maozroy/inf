/*****************************************/
/* OL79                                  */
/* ds_UID                                */
/* 17/11/19                              */
/* Author- Sharon Rottner                */
/* Reviewer- Maoz Roytman                */
/*****************************************/

#include <stddef.h> /*size_t*/
#include <time.h>  /* time */
#include <unistd.h> /*getpid*/

#include "uid.h"

const ilrd_uid_t BAD_UID = {0, -1, 0};

ilrd_uid_t UIDCreate(void)
{
	ilrd_uid_t new_uid = {0};
	static size_t counter = 0;
	 
	new_uid.pid = getpid();
	new_uid.time = time(NULL);
	new_uid.counter = counter;
	++counter;
	
	return new_uid;
}

int UIDIsSame(ilrd_uid_t uid1, ilrd_uid_t uid2)
{
	int result = 0;
	
	if (UIDIsBad(uid1) || UIDIsBad(uid2))
	{
		result = -1;
	}
		
	else if ((uid1.pid == uid2.pid) && (uid1.time == uid2.time) && 
			(uid1.counter == uid2.counter))
	{
		result = 1;
	}
	
	return result;	
}

int UIDIsBad(ilrd_uid_t uid)
{
	return (BAD_UID.time == uid.time);
}
