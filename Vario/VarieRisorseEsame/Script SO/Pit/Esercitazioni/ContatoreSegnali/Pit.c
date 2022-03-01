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

#define MAX_MITTENTI 3
typedef struct Contatore{
    int pid;
    int usr1;
    int usr2;
}Contatore;

Contatore contatore[MAX_MITTENTI]; 
struct sigaction sa;
int terminazione=1;
int cont=0;

void stampa(){
    printf(" ---- Contatore ----\n");
    for(int i=0; i<MAX_MITTENTI; i++){
        printf("[ %d SIGUSR1 (%d) SIGUSR2 (%d) ]\n", contatore[i].pid, contatore[i].usr1, contatore[i].usr2);
    }
}
int check(int pid){
    for(int i=0; i<MAX_MITTENTI; i++){
        if(pid == contatore[i].pid){
            return i;
        }
    }
    return -1;
}
void inserisci(int pid){
    contatore[cont].pid=pid;
    contatore[cont].usr1=0;
    contatore[cont].usr2=0;
    cont=cont+1;
}

void funzione(int signo, siginfo_t * info, void *empty){
    int pid=info->si_pid;

    int pos = check(pid); 
    if(pos==-1 && cont < MAX_MITTENTI){ 
        inserisci(pid); pos=cont-1;
    }
    if(pos==-1 && cont == MAX_MITTENTI){
        stampa();
        terminazione=0;
    }
    if(signo==SIGUSR1){
        printf("SIGUSR1 (%d) ricevuto\n", SIGUSR1);
        int value = contatore[pos].usr1;
        contatore[pos].usr1 = value+1;
    }
    if(signo==SIGUSR2){
        printf("SIGUSR2 (%d) ricevuto\n", SIGUSR2);
        int value = contatore[pos].usr2;
        contatore[pos].usr2 = value+1;
    }
    if(signo==SIGTERM){
        printf("SIGTERM (%d) ricevuto\n", SIGTERM);
        stampa();
        terminazione=0;
    }
} 


int main(int argc, char **argv){ 
    printf("PID [Padre]: %d\n", getpid());
    int processo;
    sa.sa_sigaction = funzione;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags |= SA_SIGINFO;
    sigaction(SIGUSR1, &sa, NULL);
    sigaction(SIGUSR2, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);
    while(terminazione);

    return 0;
}