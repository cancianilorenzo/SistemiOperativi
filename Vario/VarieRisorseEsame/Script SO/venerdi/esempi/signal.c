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


void handlerSigUsr1(int sigNum){
    printf("Handler SIGUSR1\n");
    exit(0);
}

int main(int argc, char** argv){
    
    int figlio = fork();
    if (figlio != 0){
        printf("Padre -->%i\n", getpid());
        signal(SIGUSR1, SIG_IGN);
    }
    else{
        printf("Figlio -->%i\n", getpid());
        signal(SIGUSR1, handlerSigUsr1);
        while(1);
    }
    while(wait(NULL)>0);
    return 0;
}