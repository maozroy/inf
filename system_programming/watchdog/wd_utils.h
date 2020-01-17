#include <signal.h>
#include <stdlib.h>

#include "scheduler.h"

static int DNR = 0;

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
