/****************************************************************************
*                                                                           *
*                               OL79 - 18/11/19                             *
*                                                                           *	
*                               Task                                       *
*                                                                           *
*****************************************************************************/
#ifndef ILRD_TASK_H
#define ILRD_TASK_H
#include <stddef.h>
#include "uid.h"

typedef struct p_queue p_queue_t;
typedef int(*task_func)(void *task_param);

 ** move to your .c file	** 
typedef struct task task_t
/*
struct task
{
	uid_t task_id;
	time_t time_to_run;
	time_t interval;
	task_func func_ptr;
	void *func_param;
};
*/



/*	This Function creates a Task
*	The function receives Interval, Func ptr and func Param.
*	task_func will return 0 if task is completed, 
*   and return 1 if repeat is needed.
*	Return Value: task_t.
*   if the function fails (UID failed / malloc failed), it returns NULL. 
				  
*	Notice: user must supply a valid task_func, func_param 
	or negative interval */
task_t *TaskCreate(time_t interval, task_func func_ptr, void* func_param);

/*  This function removes a task which was created by TaskCreate. 
*	the function de-alloctes memory. */
void TaskRemove(task_t *task);

/*	The function recieves a task and returns correclating UID,
*	Return Value: UID
*	Notice: The task cannot be NULL */
uid_t TaskGetUID(const task_t *task);

/* This function updates time_to_run of task, according to interval + now.
   Notice: The task cannot be NULL */
void TaskUpdateTime(task_t *task);

/* This function returns the value of time_to_run.
   Notice: The task cannot be NULL */
time_t TaskGetTimeToRun(task_t *task);

/* This function recieves uid and task. 
   Func will compare uid and task->uid, and returns 1 if match, or 0 if not.
   Notice: The task cannot be NULL */
int TaskFindByUID(uid_t uid_to_find, task_t *task);

/* This function recieves task and runs the function of the task. 
   Function will return the return of task_func.
   Notice: The task cannot be NULL */
int TaskRun(task_t *task);


#endif
