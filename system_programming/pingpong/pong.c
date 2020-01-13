#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/wait.h>


void PongHandler(int sig, siginfo_t *sig_struct, void *context)
{
	printf("Pong\n");
}


int main()
{
	pid_t pid = {0};
	struct sigaction pong_handler;
	
	sigemptyset(&(pong_handler.sa_mask));

	pong_handler.sa_sigaction = PongHandler;
	pong_handler.sa_flags = SA_SIGINFO;
	sigaction(SIGUSR2, &pong_handler, NULL);
	
	while (1)
	{
		sleep(2);
		kill(getppid(), SIGUSR1);
		pause();
	}
	
}



