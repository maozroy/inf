#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <fcntl.h>

sem_t *sem = (void *)0x12;
int flag = 1;

void PingHandler(int sig, siginfo_t *sig_struct, void *context)
{
	sem_post(sem);			

	flag = 1;
			write(0,"  Ping\n",8);
	(void)sig;
	(void)sig_struct;
	(void)context;
	sem_wait(sem);
}


int main()
{
	pid_t pid = {0};
	struct sigaction ping_handler;
	char *name = "sem11";

	sem = sem_open(name, O_CREAT, 0777, 0);
	
	sigemptyset(&(ping_handler.sa_mask));
		ping_handler.sa_sigaction = PingHandler;
		ping_handler.sa_flags = SA_SIGINFO;
		sigaction(SIGUSR1, &ping_handler, NULL);
	
	pid = fork();
	if (pid > 0)
	{


	}
	if (pid == 0)
	{
		execvp("./pong.out", NULL);
	}
	sleep(1);

	while (1)
	{

	



			kill(pid, SIGUSR2);
			flag = 0;
		


	}
	
}



