#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <stdio.h>

#include "scheduler.h"

extern size_t signal_counter_thread_g;
extern size_t signal_counter_proc_g;
extern int DNR;


typedef enum 
{
	SUCCESS = 0,
	FAIL = 1
}status_t;

typedef struct
{
	int dead_time;
	scheduler_t *sched;
}is_alive_param_t;



void ResetThreadCounter(int nothing);
void ResetProcCounter(int nothing);
int IsProcAlive(void *data);
int IsThrdAlive(void *data);
int DNRCheck(void *data);
int SendSIGUSR1(void *proc_id);
pid_t CreateWD(char *file, char *argv[]);
