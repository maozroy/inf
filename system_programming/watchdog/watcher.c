#define _POSIX_C_SOURCE 200112L

#include <signal.h>
#include <pthread.h>
#include <fcntl.h>         
#include <sys/stat.h>     
#include <semaphore.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "wd_utils.h"
#include "watcher.h"
#include "scheduler.h"



#include "uid.h"




typedef struct
{
	char **argv;
	status_t *status;
}thread_vars;




static void *ThreadRoutineIMP(void *vars);
void ThreadClosed(sem_t *thread_status, status_t *status);

int Mmi(char *argv[], int interval, int dead_time)
{
	pthread_t thread = {0};
	status_t status = 0;
	sem_t *thread_status = NULL;
	thread_vars variables = {0};
	char dead_time_buffer[8] = {0};
	char interval_buffer[8] = {0};	
	char app_id_buffer[8] = {0};	
	
	variables.argv = argv;
	variables.status = &status;
	
	if(-1 == sprintf(interval_buffer, "%d", interval))
	{
		return FAIL;
	}
	if(-1 == sprintf(app_id_buffer, "%d", getpid()))
	{
		return FAIL;
	}
	
	if(-1 == sprintf(dead_time_buffer, "%d", dead_time))
	{
		return FAIL;
	}	
	
	setenv("interval",interval_buffer, 0);
	setenv("dead_time",dead_time_buffer, 0);	
	setenv("app_id",app_id_buffer, 0);		
	
	thread_status = sem_open("thread_status", O_CREAT, 0777, 0);
	if(SEM_FAILED == thread_status)
	{
		return FAIL;
	}
	
	status = pthread_create(&thread, NULL, ThreadRoutineIMP, (void *)&variables);
	if (FAIL == status)
	{
		sem_close(thread_status);
		return status;
	}

	sem_wait(thread_status);
	sem_close(thread_status);
	
	return status;
}


static void *ThreadRoutineIMP(void *vars)
{
	sem_t *thread_status = NULL;
	sem_t *thread_ready = NULL;
	sem_t *wtchdg_ready = NULL;
	pid_t pid_to_signal = {0};
	struct sigaction counter_handle = {0};
	scheduler_t *sched = NULL;
	int interval = 0;
	int dead_time = 0;
	char *argv = NULL;
	status_t *status = NULL;
	thread_vars *variables = vars;
	ilrd_uid_t task_id = {0};
	is_alive_param_t is_alive_params = {0};
	
	interval = atoi(getenv("interval"));
	dead_time = atoi(getenv("dead_time"));
	

	
	status = variables -> status;
	
	thread_status = sem_open("thread_status", O_CREAT, 0777, 0);
	if (SEM_FAILED == thread_status)
	{
		free(argv);
		ThreadClosed(thread_status, status);
	}
	thread_ready = sem_open("thread_ready", O_CREAT, 0777, 0);
	if (SEM_FAILED == thread_ready)
	{
		free(argv);
		sem_close(thread_status);
		ThreadClosed(thread_status, status);
	}	
	wtchdg_ready = sem_open("wtchdg_ready", O_CREAT, 0777, 0);
	if (SEM_FAILED == wtchdg_ready)
	{
		free(argv);
		sem_close(thread_status);
		sem_close(thread_ready);
		ThreadClosed(thread_status, status);
	}	
	counter_handle.sa_flags = 1;
	counter_handle.sa_handler = ResetThreadCounter;
	
	if(-1 == sigaction(SIGUSR1, &counter_handle, NULL))
	{
		free(argv);
		sem_close(thread_status);
		sem_close(wtchdg_ready);
		sem_close(thread_ready);		
		ThreadClosed(thread_status, status);	
	}
	sched = SchedCreate();
	if(NULL == sched)
	{
		free(argv);
		sem_close(thread_status);
		sem_close(wtchdg_ready);
		sem_close(thread_ready);		
		ThreadClosed(thread_status, status);	
	}
	
	is_alive_params.dead_time = dead_time;
	is_alive_params.sched = sched;
	task_id = SchedAdd(sched, interval, IsProcAlive, &is_alive_params);
	if (UIDIsBad(task_id) == 0)
	{
		free(argv);
		sem_close(thread_status);
		sem_close(wtchdg_ready);
		sem_close(thread_ready);		
		SchedStop(sched);
		ThreadClosed(thread_status, status);		
	}
	task_id = SchedAdd(sched, interval, SendSIGUSR1, &pid_to_signal);
	
	while (DNR)
	{
	
	
	}
	
	free(argv);
	sem_post(thread_status);
	
	
	
	return NULL;
}




void ThreadClosed(sem_t *thread_status, status_t *status)
{
	*status = FAIL;
	sem_post(thread_status);
	pthread_exit(NULL);
}



