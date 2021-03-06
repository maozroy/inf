/****************************************************************************
*                                                                           *
*                               OL79 - 17/11/19                             *
*                                                                           *	
*                                   Uniqe Id                                *
*                                                                           *
*****************************************************************************/

#ifndef ILRD_UID_H
#define ILRD_UID_H

#include <stddef.h>
#include <time.h>
#include <sys/types.h>

typedef struct uid_pid_time_counter
{
	pid_t pid;
	time_t time;
	size_t counter;
	
} ilrd_uid_t;

extern const ilrd_uid_t BAD_UID;

/*
The  function creates a new UID of Counter, time stamp & Process number
If the UID fails, the function will return BAD_UID
It is receommended to check for BAD_UID outcome with every create
*/
ilrd_uid_t UIDCreate(void);

/*
The  function checks if 2 UIDs are teh same or not
If they are the same, the function will return 1
If they are the differenr, the function will return 0
If one of the UIDs received are not valid or NULL, the function will return -1
*/
int UIDIsSame(ilrd_uid_t uid1, ilrd_uid_t uid2);

/*
The function checks if the UID is valid
If the UID is valid, the function will return 1
If the UID is bad or NULL, the function will return 0
*/
int UIDIsBad(ilrd_uid_t uid);

#endif
