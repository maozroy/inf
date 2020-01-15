#define _POSIX_C_SOURCE 1
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <errno.h>

#define errExit(msg) do { perror(msg); exit(EXIT_FAILURE); } while (0)

void sig_hand(int sig) {}

sigset_t saveMask, blockMask;

void child_process()
{

	while(1)
	{
	   	sleep(1);  	
		if (sigsuspend(&saveMask) == -1 && errno != EINTR)
	  	{
		    errExit("sigsuspend");
		}
		
        printf("Pong!\n");
        kill(getppid(), SIGUSR1);
    }

    return ;
}

void parent_process(pid_t pid)
{
    int y = 0;

    while (1)
    {

        printf("Ping!\n");

        kill(pid, SIGUSR1);
      if (sigsuspend(&saveMask) == -1 && errno != EINTR)
      {
            errExit("sigsuspend");
      }
    }

    return ;
}


int main(int argc, char* argv[])
{
    struct sigaction sa;
       pid_t pid = {0};

    sigemptyset(&blockMask);
    sigaddset(&blockMask, SIGUSR1);

    if (sigprocmask(SIG_BLOCK, &blockMask, &saveMask) == -1)
        errExit("sigprocmask");



	sigemptyset(&sa.sa_mask);  
    sa.sa_flags = 0;
    sa.sa_handler = sig_hand;

    if (sigaction(SIGUSR1, &sa, NULL) == -1)
        errExit("sigaction");

    pid = fork();

    if (pid == 0)
        child_process();
    else
        parent_process(pid);

    return 0;
}
