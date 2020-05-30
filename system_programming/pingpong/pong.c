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
int flag = 0;

void PongHandler(int sig, siginfo_t *sig_struct, void *context)
{
	sem_post(sem);

	flag = 1;
			write(0,"Pong\n",6); 
	(void)sig;
	(void)sig_struct;
	(void)context;
	sem_wait(sem);
}


int main()
{
	struct sigaction pong_handler;
	char *name = "sem11";
	sem = sem_open(name, O_CREAT , 0777, 0);
	sigemptyset(&(pong_handler.sa_mask));

	pong_handler.sa_sigaction = PongHandler;
	pong_handler.sa_flags = SA_SIGINFO;
	sigaction(SIGUSR2, &pong_handler, NULL);
							sem_wait(sem);
	while (1)
	{

	



			kill(getppid(), SIGUSR1);		
			flag = 0;
		
	}
	
}



