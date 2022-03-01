#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include <errno.h>
#include <sys/wait.h>

int figlio1, figlio2, padre;

int main(int argc, char **argv)
{
    padre = getpid();

    signal(SIGUSR1, SIG_IGN);

    figlio1 = fork();
    if (figlio1 == 0)
    {
        //Sono nel figlio
        setpgid(getpid(), padre);
        signal(SIGUSR1, SIG_DFL);
        printf("[PID]: %i, [GID]: %i\n", getpid(), getpgid(getpid()));
        while (1)
            ;
    }
    if (figlio1 != 0)
    {
        figlio2 = fork();
        if (figlio2 == 0)
        {
            //Sono nel figlio
            setpgid(getpid(), padre);
            signal(SIGUSR1, SIG_DFL);
            printf("[PID]: %i, [GID]: %i\n", getpid(), getpgid(getpid()));
            while (1)
                ;
        }
        //printf("-----[PID]: %i-----\n", figlio1);
        setpgid(getpid(), getpid());
        printf("[PID]: %i, [GID]: %i\n", getpid(), getpgid(getpid()));
    }

    while (wait(NULL) > 0)
        ;
    return 0;
}