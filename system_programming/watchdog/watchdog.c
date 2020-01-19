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
#include "scheduler.h"
#include "uid.h"

int main(int argc, char *argv[])
{
	sem_t *thread_status = NULL;
	sem_t *thread_ready = NULL;
	sem_t *wtchdg_ready = NULL;
	pid_t pid_to_signal = {0};
	struct sigaction counter_handle = {0};
	scheduler_t *sched = NULL;
	int interval = 0;
	int dead_time = 0;
	ilrd_uid_t task_id = {0};
	is_alive_param_t is_alive_params = {0};
	char *app_id = NULL;
	struct timespec time_struct = {0}; 

	app_id = getenv("app_id");
	pid_to_signal = atoi(app_id);
	printf("app_id is %s\n", app_id);
	interval = atoi(getenv("interval"));
	dead_time = atoi(getenv("dead_time"));
	time_struct.tv_sec = interval * dead_time;

	
	thread_status = sem_open("thread_status", O_CREAT, 0777, 0);
	if (SEM_FAILED == thread_status)
	{
		return FAIL;
	}
	thread_ready = sem_open("thread_ready", O_CREAT, 0777, 0);
	if (SEM_FAILED == thread_ready)
	{
		sem_unlink("thread_status");
		return FAIL;
	}	
	wtchdg_ready = sem_open("wtchdg_ready", O_CREAT, 0777, 0);
	if (SEM_FAILED == wtchdg_ready)
	{
		sem_unlink("thread_status");
		sem_unlink("thread_ready");
		return FAIL;
	}	
	counter_handle.sa_flags = 1;
	counter_handle.sa_handler = ResetThreadCounter;
	
	if(-1 == sigaction(SIGUSR1, &counter_handle, NULL))
	{
		sem_unlink("thread_status");
		sem_unlink("wtchdg_ready");
		sem_unlink("thread_ready");		
		return FAIL;
	}
	sched = SchedCreate();
	if(NULL == sched)
	{
		sem_unlink("thread_status");
		sem_unlink("wtchdg_ready");
		sem_unlink("thread_ready");		
		return FAIL;
	}

	is_alive_params.dead_time = dead_time;
	is_alive_params.sched = sched;
		if (UIDIsBad(task_id) == 1)
	{
		sem_unlink("thread_status");
		sem_unlink("wtchdg_ready");
		sem_unlink("thread_ready");		
		SchedDestroy(sched);
		return FAIL;
	}
	
	task_id = SchedAdd(sched, interval, IsThrdAlive, &is_alive_params);
	if (UIDIsBad(task_id) == 1)
	{
		sem_unlink("thread_status");
		sem_unlink("wtchdg_ready");
		sem_unlink("thread_ready");		
		SchedDestroy(sched);
		return FAIL;
	}
	task_id = SchedAdd(sched, interval, SendSIGUSR1, &pid_to_signal);

	task_id = SchedAdd(sched, interval, DNRCheck, sched);
	if (UIDIsBad(task_id) == 1)
	{
		sem_unlink("thread_status");
		sem_unlink("wtchdg_ready");
		sem_unlink("thread_ready");			
		SchedDestroy(sched);
		return FAIL;
	}
	/*
	if (getppid() != pid_to_signal)
	{
			pid_to_signal = CreateWD("wd.out", &argv);	

		if (0 == pid_to_signal)
		{

			free(argv);
			sem_unlink("thread_status");
			sem_unlink("wtchdg_ready");
			sem_unlink("thread_ready");		
			SchedDestroy(sched);
			return FAIL;
		}
	}
	*/
	printf("watchdog init\n");
	while (DNR == 0)
	{/*
		if (pid_to_signal != getppid())
		{
			pid_to_signal = CreateWD("wd.out", &argv);	
			
			if (0 == pid_to_signal)
			{
				free(argv);
				sem_unlink("thread_status");
				sem_unlink("wtchdg_ready");
				sem_unlink("thread_ready");	
				SchedDestroy(sched);
				ThreadClosed(thread_status, status);		
			}
		}*/
		sem_post(wtchdg_ready);
		if (sem_timedwait(thread_ready, &time_struct) == -1)
		{
			sem_unlink("thread_status");
			sem_unlink("wtchdg_ready");
			sem_unlink("thread_ready");	
			SchedDestroy(sched);
			return FAIL;
		}
		printf("watchdog starting\n");
					
		SchedRun(sched);
	
	}

	return 0;
}

pid_t CreateApp()
{


}
