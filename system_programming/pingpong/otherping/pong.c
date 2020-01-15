/*
gd pong.c -o pong.out
gd ping.c
./pong.out &
./a.out *PID*
*/

#define _POSIX_C_SOURCE 200899L
#include <stdio.h> /* printf */
#include <signal.h> /* sigaction */
#include <stdlib.h> /* atoi */
#include <unistd.h> /* sleep */
#include <pthread.h>
#include <semaphore.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/stat.h> 

#define UNUSED(x) (void)(x)

const char *semName = "gal";
sig_atomic_t flag = 0;
static pid_t ping_id = 0;

void sig_handler(int signum, siginfo_t *info, void *uncontext)
{
    UNUSED(signum);
    UNUSED(uncontext);

    ping_id = info->si_pid;
    flag = 1;
}

int main(int argc, char *argv[])
{
    struct sigaction sa;
    sem_t *sem_id = sem_open(semName, O_CREAT, 0600, 0);

    UNUSED(argc);

    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = sig_handler;
    
    if (-1 == sigaction(SIGUSR1, &sa, NULL))
    {
        perror("sigaction child");
    }

	sem_wait(sem_id);

    while (1)
    {
    	sleep(1);
        if (1 == flag)
        {            
            puts("pong");            
            flag = 0;
        }        
        kill(ping_id, SIGUSR2);
        sem_post(sem_id);        
    }

    return 0;
}
