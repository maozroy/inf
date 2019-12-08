 /********************************************
*	Author : MaozRoytman
*	Reviewer : 	Gal Salemon
*	Task					
*	Date: Nov 23 2019		                
*																				
********************************************/

#include <stdlib.h> /*MALLOCING*/
#include <assert.h> /*asserting*/

#include "uid.h"
#include "task.h"

struct task
{
	task_id_t task_id;
	time_t time_to_run;
	time_t interval;
	task_func action_func;
	void *action_func_param;
};

task_t *TaskCreate(time_t interval, 
					task_func action_func, void* action_func_param)
{
	task_t *new_task = NULL;
	
	assert(action_func_param);

	new_task = (task_t *)malloc(sizeof(task_t)); 
	if (NULL == new_task)
	{
		return NULL;
	}
	
	new_task->action_func = action_func;
	new_task->action_func_param = action_func_param;
	new_task->interval = interval;
	new_task->task_id = UIDCreate();
	TaskUpdateTimeToRun(new_task);
	
	if (UIDIsBad(new_task->task_id))
	{
		free(new_task);
		
		return NULL;
	}
	
	return new_task;
}

time_t TaskGetTimeToRun(task_t *task)
{
	assert(task);

	return task->time_to_run;
}

void TaskUpdateTimeToRun(task_t *task)
{
	assert(task);

	task->time_to_run = task->interval + time(NULL);
}

void TaskRemove(task_t *task)
{
	free(task);
	task = NULL;
}

int TaskIsMatchByID(task_id_t id_to_find, task_t *task)
{
	assert(task);

	return UIDIsSame(id_to_find, TaskGetID(task));
}

int TaskRun(task_t *task)
{
	assert(task);

	return task->action_func(task->action_func_param);
}

task_id_t TaskGetID(const task_t *task)
{
	assert(task);

	return task->task_id;
}
