#define _POSIX_C_SOURCE

#include <pthread.h>
#include <fcntl.h>         
#include <sys/stat.h>     
#include <semaphore.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>

#include "wd_utils.h"
#include "scheduler.h"


typedef enum 
{
	SUCCESS = 0,
	FAIL = 1
}status_t;

typedef struct
{
	int interval;
	int dead_time;
	char **argv;
	status_t *status;
}thread_vars;

static void *ThreadRoutineIMP(void *vars);

int Mmi(char *argv[], int interval, int dead_time)
{
	pthread_t thread = {0};
	status_t status = 0;
	sem_t *thread_status = NULL;
	thread_vars variables = {0};
	
	variables.interval = interval;
	variables.dead_time = dead_time;
	variables.argv = argv;
	variables.status = &status;
	
	thread_status = sem_open("thread_status", O_CREAT, 0777, 0);
	if(SEM_FAILED == thread_status)
	{
		return FAIL;
	}
	
	status = pthread_create(&thread, NULL, ThreadRoutineIMP, (void *)&variables);
	if (FAIL == status)
	{
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
	int interval = 0;
	int dead_time = 0;
	char *argv = NULL;
	int argv_len = 0;
	status_t *status = NULL;
	thread_vars *variables = vars;
	
	interval = variables -> interval;
	dead_time = variables -> dead_time;
	status = variables -> status;
	
	argv_len = strlen(*(variables -> argv));
	argv = malloc(argv_len);
	if (NULL == argv)
	{
		*status = FAIL;
		return NULL;
	}
	
	memcpy(argv, *(variables -> argv), argv_len);
	
	thread_status = sem_open("thread_status", O_CREAT, 0777, 0);
	thread_ready = sem_open("thread_ready", O_CREAT, 0777, 0);
	wtchdg_ready = sem_open("wtchdg_ready", O_CREAT, 0777, 0);
	
	counter_handle.sa_flags = 0;
	counter_handle.sa_handler = SetOffCounter;
	
	sigaction(SIGUSR1, &counter_handle, NULL);
	
	
	
	free(argv);
	
	return NULL;
}
