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


//parametri info e empty solo se uso informazioni
static void handler(int signo, siginfo_t* info, void* empty){
    switch (signo) {
            case SIGUSR1:
                printf("SIGUSR1 da sigaction da [PID]: %i\n",info->si_pid );
            break;
            case SIGUSR2:
                printf("SIGUSR2 da sigaction da [PID]: %i\n",info->si_pid );
            break;
    }
}

int main(void)
{
    printf("[PID] %i\n", getpid()); fflush(stdout);
    struct sigaction sa; //struct sigaction
    sa.sa_sigaction = handler; //utilizzo informazioni
    //sa.sa_handler = handler; //non utilizzo informazioni
    sigemptyset(&sa.sa_mask);   //svuotare maschera
    sa.sa_flags |= SA_SIGINFO;  //flag per informazioni
    sigaction(SIGUSR1, &sa, NULL); //segnale gestito in handler
    sigaction(SIGUSR2, &sa, NULL);
    sigaddset(&sa.sa_mask,SIGTERM); //segnale aggiunto a maschera
    sigdelset(&sa.sa_mask,SIGTERM); //segnale rimosso a maschera

    sigprocmask(SIG_BLOCK, &sa.sa_mask, NULL); //bloccare segnali della maschera
    //sigprocmask(SIG_IGN, &sa.sa_mask, NULL); //ignorare segnali della maschera
    
    kill(getpid(),SIGTERM);
    sigpending(&sa.sa_mask); //inserisce nella maschera se un segnale è pending o no
    if(sigismember(&sa.sa_mask, SIGTERM)){ //contollo segnale è pending (ricevuto e in attesa)
        printf("SIGTERM pending\n");
        sigprocmask(SIG_UNBLOCK, &sa.sa_mask, NULL); //sblocca segnali della maschera ed esegue quelli bloccati che ha ricevuto prima
    }


    while(1);
    return 0;
}

    