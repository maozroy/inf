#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <signal.h>

#define _POSIX_SOURCE

#include "watchdog.h"
int PingPong(void);

int main()
{
		PingPong();
		
		return 0;
}


void Handler1(int signum)
{
	if (signum == SIGUSR1)
	{
		printf("pong\n");
		
	}
}

void Handler2(int signum)
{
	if (signum == SIGUSR2)
	{
		printf("ping\n");
	}
}


int PingPong(void)
{
	pid_t new_pid = 0;
	int child_status = 0;
	int i = 0;
	pid_t parent_id = 0;
		new_pid = fork();
	for(i = 0; i < 10 ; i++)
	{
	sleep(1);
	signal(SIGUSR2, Handler1);
	signal(SIGUSR1, Handler2);
	
	if (new_pid > 0)
	{
		printf("parent\n");
		kill(new_pid, SIGUSR1);
		wait(&child_status);
	}
	else if (0 == new_pid)
	{
	printf("child\n");
	parent_id = getppid();
	kill(parent_id, SIGUSR1);

	}
}
}


