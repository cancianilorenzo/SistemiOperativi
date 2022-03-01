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

static void handler(int signo, siginfo_t* info, void* empty){
    switch (signo) {
            case SIGUSR1:
                printf("SIGUSR1 da sigaction da [PID]: %i\n",info->si_pid );
                fflush(stdout);
            break;
            case SIGUSR2:
                printf("SIGUSR2 da sigaction da [PID]: %i\n",info->si_pid );
                exit(0);
            break;
    }
}

int main(void)
{
    printf("[PID] %i\n", getpid()); fflush(stdout);
    struct sigaction sa;
    sa.sa_handler = handler;
    sigemptyset(&sa.sa_mask);  
    sa.sa_flags |= SA_SIGINFO; 
    sigaction(SIGUSR1, &sa, NULL);
    sigaction(SIGUSR2, &sa, NULL);
    
    //kill(getpid(), SIGUSR1);
    //kill(getpid(), SIGUSR2);
    
    while(1);
    return 0;
}