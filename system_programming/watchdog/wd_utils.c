#include "wd_utils.h"

size_t signal_counter_thread_g = 0;
size_t signal_counter_proc_g = 0;

void ResetThreadCounter(int nothing)
{
	(void)nothing;
	signal_counter_thread_g = 0;
}

void ResetProcCounter(int nothing)
{
	(void)nothing;
	signal_counter_proc_g = 0;
}

int IsProcAlive(void *data)
{
	++signal_counter_proc_g;
	if (((is_alive_param_t *)data) -> dead_time >= signal_counter_proc_g )
	{
		SchedStop(((is_alive_param_t *)data)-> sched);
	}
	
	return 1;
}

int IsThrdAlive(void *data)
{
	++signal_counter_thread_g;
	if (((is_alive_param_t *)data) -> dead_time >= signal_counter_thread_g )
	{
		SchedStop(((is_alive_param_t *)data)-> sched);
	}
	
	return 1;
}

int DNRCheck(void *data)
{
	if(DNR == 1)
	{
		SchedStop((scheduler_t *)data);
	}
	
	return 1;
}

int SendSIGUSR1(void *proc_id)
{
	kill(*(pid_t *)proc_id, SIGUSR1);

	return 1;
}
