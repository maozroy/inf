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

sig_atomic_t flag = 1;
const char *semName = "gal";

void sig_handler(int signum)
{
    UNUSED(signum);

    flag = 1;
}

int main(int argc, char *argv[])
{
    struct sigaction sa;
    pid_t pong_pid = (pid_t)atoi(argv[1]);
	sem_t *sem_id = sem_open(semName, O_CREAT);

    UNUSED(argc);

    sa.sa_flags = 0;
    sa.sa_handler = sig_handler;

    if (-1 == sigaction(SIGUSR2, &sa, NULL))
    {
        perror("sigaction child");
    }

	/*sem_post(sem_id);*/
	
    while (1)
    {
    sleep(1);
        if (1 == flag)
        {
            puts("ping");
            flag = 0;
        }
        kill(pong_pid, SIGUSR1);
        sem_wait(sem_id);
    }

    return 0;
}
