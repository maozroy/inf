 /********************************************
*	Author : MaozRoytman
*	Reviewer : 	Gal Salemon
*	Scheduler
*	Date: Nov 23 2019		                
*																				
********************************************/

#include <stdlib.h> /*MALLOCING*/
#include <assert.h> /*asserting*/
#include <unistd.h> /*sleep*/

#include "../../ds/uid/uid.h"
#include "../../ds/pq/pq.h"
#include "../../ds/task/task.h"
#include "scheduler.h"

struct scheduler
{
	p_queue_t *pq;
	int stop_flag;
	int has_removed_itself;
	task_t *running_task;
};

/*compare and match funcs for pq implemenation*/
static int MyCompareIMP (const void *new_data,
						 const void *src_data, void *param);
static int MyIsMatchIMP (const void *new_data, const void *param);

/*basic MAX func for time*/
static size_t maxIMP (time_t x, time_t y);

scheduler_t *SchedCreate(void)
{
	scheduler_t *new_sched = (scheduler_t *)malloc(sizeof(scheduler_t));
	if (NULL == new_sched)
	{
		return NULL;
	}
	
	new_sched->pq = PQCreate(NULL, MyCompareIMP);
	if (NULL == new_sched->pq)
	{
		free(new_sched);
		return NULL;
	}
	
	new_sched->has_removed_itself = 0;
	new_sched->running_task = NULL;
	
	return new_sched;		
}

static int MyCompareIMP 
		   (const void *new_data, const void *src_data, void *param)
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
	scheduler = NULL;
}

ilrd_uid_t SchedAdd(scheduler_t *scheduler, time_t interval,
					action_func action, void *action_func_param)
 {
 	task_t *new_task = NULL;
 	
 	assert(action);
 	assert(scheduler);
 	
 	new_task = TaskCreate(interval, action, action_func_param);
 	if (NULL == new_task)
 	{
 		return BAD_UID;	
 	}
 	
 	if (PQEnqueue(scheduler->pq, new_task))
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

	task_to_remove = (PQErase(&event_id, scheduler->pq, MyIsMatchIMP));
	if (NULL != task_to_remove)
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
		else
		{
			result = 1;
		}
	}
	
	return result;
}

static int MyIsMatchIMP(const void *new_data, const void *param)
{
	return (TaskIsMatchByID(*(ilrd_uid_t *)param, (task_t *)new_data));
}

enum result_status SchedRun(scheduler_t *scheduler)
{
	int is_repeat = 0;
	int result_status = 0;
	time_t time_to_sleep = 0;
	
	assert(scheduler);
	
	scheduler->stop_flag = 0;
	scheduler->has_removed_itself = 0;

	while (!(scheduler->stop_flag) && (!SchedIsEmpty(scheduler)))
	{
		scheduler->running_task = PQDequeue(scheduler->pq);
		time_to_sleep = maxIMP(difftime(TaskGetTimeToRun
						(scheduler->running_task), time(NULL)), 0);
							
		sleep(time_to_sleep);
		is_repeat = TaskRun(scheduler->running_task);
		
		if (is_repeat && !(scheduler->has_removed_itself))
		{
			TaskUpdateTimeToRun(scheduler->running_task);
			
			if (PQEnqueue(scheduler->pq, scheduler->running_task))
			{
				result_status = ENQUEUE_FAILED;
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
		result_status = SCHEDULER_EMPTY;
	}
	else
	{
		result_status = STOPPED_SUCCESSFULLY;
	}
	
	return result_status;
}

void SchedStop(scheduler_t *scheduler)
{
	scheduler->stop_flag = 1;
}

void SchedClear(scheduler_t *scheduler)
{
	assert(scheduler);

	while (!(SchedIsEmpty(scheduler)))
	{
		if ((NULL != scheduler->running_task) && (1 == SchedSize(scheduler)))
		{		
				scheduler->has_removed_itself = 1;
				break;
		}
		TaskRemove(PQDequeue(scheduler->pq));
	}
}

int SchedIsEmpty(const scheduler_t *scheduler)
{
	assert(scheduler);

	if (NULL != scheduler->running_task)
	{
		return 0;
	}
	
	return PQIsEmpty(scheduler->pq);
}

size_t SchedSize(const scheduler_t *scheduler)
{
	size_t size = 0;
	
	assert(scheduler);
	
	size = PQSize(scheduler->pq);
	
	if (NULL != scheduler->running_task)
	{
		size += 1;
	}

	return size;
}

static size_t maxIMP(time_t x, time_t y)
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
