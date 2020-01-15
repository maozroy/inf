#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "watchdog.h"
int ForkWatchDog(char *program, char **arg_list);
int SysWatchDog(char *program, char **arg_list);

int main(int argc, char *argv[])
{
		WatchDog(argv[2], &argv[1]);
		(void)argc;
		
		return 0;
}

int WatchDog(char *program, char **arg_list)
{
	if (0 == strcmp("fork", arg_list[0]))
	{
		ForkWatchDog(program, &arg_list[1]);
	}
	else if (0 == strcmp("system", arg_list[0]))
	{
		SysWatchDog(program, &arg_list[1]);
	}
	else
	{
		printf("Wrong Input!\n");
	}
	return 0;
}

int ForkWatchDog(char *program, char **arg_list)
{
	pid_t new_pid = 0;
	int child_status = 0;
	
	while (1)
	{
		new_pid = fork();
		sleep(1);
		if (new_pid > 0)
		{
			printf("pid is %d\n",new_pid);
			wait(&child_status);
		}
		else if (0 == new_pid)
		{
			if (execvp(program, arg_list) < 0)
			{
				printf ("exec failed!\n");
			}
		}
	}
}

int SysWatchDog(char *program, char **arg_list)
{
	char command[100] = {0};
	(void) program;
	
	while (NULL != *arg_list)
	{
		strcat(command, *arg_list);
		++arg_list;
		strcat(command, " ");
	}	
	
	while (1)
	{
		sleep(1);
		system(command);
	}
	return 0;
}
