/************************************
*		Author: ChenR				  *
*		Reviewer: 					  *
*		scheduler					  *
*		7/11/2019					  *
*									  *
************************************/
#include <time.h> 	/* time_t */
#include <stdlib.h>	/* malloc */
#include <assert.h> /* assert */
#include <stdio.h> /* assert */

#include "uid.h"
#include "task.h"

#define CURRENT_TIME time(NULL)

struct task
{
	task_id_t task_id;
	time_t time_to_run;
	time_t interval;
	task_func action_func;
	void *action_func_param;
};

task_t *TaskCreate(time_t interval, task_func action_func, 
				   void* action_func_param)
{
	task_t *new_task = (task_t *)malloc(sizeof(task_t));
	if (NULL == new_task)
	{
		return NULL;
	}
	
	new_task -> interval = interval;
	new_task -> action_func = action_func;
	new_task -> action_func_param = action_func_param;
	TaskUpdateTimeToRun(new_task);
	new_task -> task_id = UIDCreate();
	
	if (UIDIsBad(new_task -> task_id))
	{
		free(new_task);
		
		return NULL;
	}
	
	return new_task;
}

void TaskRemove(task_t *task)
{
	assert(task);
	
	free(task);
	task = NULL;
}

void TaskUpdateTimeToRun(task_t *task)
{
	assert(task);
	
	task -> time_to_run = CURRENT_TIME + task -> interval;
}

task_id_t TaskGetID(const task_t *task)
{
	assert(task);
	
	return task -> task_id;
}

time_t TaskGetTimeToRun(task_t *task)
{
	assert(task);
	
	return task -> time_to_run;
}

int TaskIsMatchByID(task_id_t id_to_find, task_t *task)
{
	assert(task);
	
	return (UIDIsSame(id_to_find, task -> task_id));
}

int TaskRun(task_t *task)
{
	assert(task);
	return (task -> action_func(task -> action_func_param));
}
