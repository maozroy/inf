 /********************************************
*	Author : 			
*	Reviewer : 	
*	task					
*	Date: Wed Nov 20 08:42:19 IST 2019		                
*																				
********************************************/

#include <stdlib.h> /*MALLOCING*/
#include <assert.h> /*asserting*/
#include <unistd.h>
#include <stdio.h>

#include "uid.h"
#include "pq.h"
#include "task.h"
#include "scheduler.h"

struct scheduler
{
	p_queue_t *pq;
	int stop_flag;
	int has_removed_itself;
	task_t *running_task;
};

static int MyCompare (const void *new_data, const void *src_data, void *param);
static int MyIsMatch(const void *new_data, const void *param);
static size_t max(time_t x, time_t y);

scheduler_t *SchedCreate(void)
{
	scheduler_t *new_sched = (scheduler_t *)malloc(sizeof(scheduler_t));
	if (NULL == new_sched)
	{
		return NULL;
	}
	
	new_sched->pq = PQCreate(NULL, MyCompare);
	if (NULL == new_sched->pq)
	{
		free(new_sched);
		return NULL;
	}
	new_sched->has_removed_itself = 0;
	new_sched->running_task = NULL;
	
	return new_sched;		
}

static int MyCompare (const void *new_data, const void *src_data, void *param)
{
	double diff = difftime(TaskGetTimeToRun((task_t *)new_data), 
										TaskGetTimeToRun((task_t *)src_data));
	(void) param;
	if (diff < 0 )
	{
		return 1;
	}
	return 0;
}

void SchedDestroy(scheduler_t *scheduler)
{
	assert(scheduler);
	
	SchedClear(scheduler);
	PQDestroy(scheduler->pq);
	free(scheduler);
}

ilrd_uid_t SchedAdd(scheduler_t *scheduler, time_t interval,
					action_func action, void *action_func_param)
 {
 	int result = 0;
 	task_t *new_task = NULL;
 	
 	assert(action);
 	
 	new_task = TaskCreate(interval, action, action_func_param);
 	if (NULL == new_task)
 	{
 		return BAD_UID;	
 	}
 	result = PQEnqueue(scheduler->pq, new_task);
 	if (result)
 	{
 		TaskRemove(new_task);
 		
 		return BAD_UID;
 	} 	
 	
 	return TaskGetID(new_task);
 }


int SchedRemove(scheduler_t *scheduler, ilrd_uid_t event_id)
{
	void *task_to_remove = NULL;
	int result = 0;

	task_to_remove = (PQErase(&event_id, scheduler->pq, MyIsMatch));
	if(NULL != task_to_remove)
	{
		TaskRemove(task_to_remove);
		
		result = 0;
	}
	else
	{
		if (NULL == scheduler->running_task)
		{
			result = 1;
		}
		else if (TaskIsMatchByID(event_id, scheduler->running_task))
		{
			scheduler->has_removed_itself = 1;
			result = 0;
		}
		
		result = 1;
	}
	
	return result;
}


static int MyIsMatch(const void *new_data, const void *param)
{
	return TaskIsMatchByID(*(ilrd_uid_t *)param, (task_t *)new_data);
}


enum result_status SchedRun(scheduler_t *scheduler)
{
	int repeat = 0;
	int result = 0;
	time_t time_to_sleep = 0;
	scheduler->stop_flag = 0;


	
	while(!(scheduler->stop_flag) && !(SchedIsEmpty(scheduler)))
	{
		scheduler->running_task = PQDequeue(scheduler->pq);
		scheduler->has_removed_itself = 0;
		time_to_sleep = max(difftime(TaskGetTimeToRun(scheduler->running_task)
							, time(NULL)), 0);
		sleep(time_to_sleep);
		repeat = TaskRun(scheduler->running_task);
		
		if (repeat && !(scheduler->has_removed_itself))
		{
			TaskUpdateTimeToRun(scheduler->running_task);
			if(PQEnqueue(scheduler->pq, scheduler->running_task))
			{
				result = ENQUEUE_FAILED;
				break;
			}
			scheduler->running_task = NULL;
		}
		else
		{
			TaskRemove(scheduler->running_task);
			scheduler->has_removed_itself = 0;
			scheduler->running_task = NULL;
		}
	}
	
	if (SchedIsEmpty(scheduler))
	{
		result = SCHEDULER_EMPTY;
	}
	else
	{
		result = SUCCESS_STOP;
	}
	return result;
}


void SchedStop(scheduler_t *scheduler)
{
	scheduler->stop_flag = 1;
}
  

void SchedClear(scheduler_t *scheduler)
{
	while(!(SchedIsEmpty(scheduler)) )
	{
		if ( (NULL != scheduler->running_task) && (1 == SchedSize(scheduler)) )
		{		
				scheduler->has_removed_itself = 1;
				break;
		}
		TaskRemove(PQDequeue(scheduler->pq));
	}
}

int SchedIsEmpty(const scheduler_t *scheduler)
{
	if (NULL != scheduler->running_task)
	{
		return 0;
	}
	
	return PQIsEmpty(scheduler->pq);
}

size_t SchedSize(const scheduler_t *scheduler)
{
	int result = PQSize(scheduler->pq);
	
	if (NULL != scheduler->running_task)
	{
		result += 1;
	}

	return result;
}

static size_t max(time_t x, time_t y)
{
	if (x > y)
	{
		return x;
	}
	else
	{
		return y;
	}
}
