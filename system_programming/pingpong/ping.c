#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/wait.h>

void PingHandler(int sig, siginfo_t *sig_struct, void *context)
{
	printf("Ping\n");
}


int main()
{
	pid_t pid = {0};
	struct sigaction ping_handler;
	
	sigemptyset(&(ping_handler.sa_mask));

	
	pid = fork();
	if (pid > 0)
	{
		ping_handler.sa_sigaction = PingHandler;
		ping_handler.sa_flags = SA_SIGINFO;
		sigaction(SIGUSR1, &ping_handler, NULL);
	}
	if (pid == 0)
	{
		execvp("./pong.out", NULL);
	}
	
	while (1)
	{
		sleep(2);
		kill(pid, SIGUSR2);
		pause();
	}
	
}



