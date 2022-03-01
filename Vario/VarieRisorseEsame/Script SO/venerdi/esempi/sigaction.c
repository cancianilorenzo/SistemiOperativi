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

void mioHandler(int sigNum, siginfo_t* info, void* empty)
{
    if(sigNum == SIGUSR1){
        printf("SIGUSR1 da sigaction da [PID]: %i\n",info->si_pid );
    }
    if(sigNum == SIGUSR2){
        printf("SIGUSR2 da sigaction da [PID]: %i\n",info->si_pid );
        exit(0);
    }
}

int main(int argc, char **argv)
{
    printf("[PID] %i\n", getpid());
    struct sigaction su1;
    su1.sa_sigaction = mioHandler;
    sigemptyset(&su1.sa_mask);
    su1.sa_flags |= SA_SIGINFO;

    sigaction(SIGUSR1, &su1, NULL);
    sigaction(SIGUSR2, &su1, NULL);
    while (1);
}