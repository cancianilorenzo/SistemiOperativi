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

int main(int argc, char **argv){
    int status;
    int processo;
    int figlio;
    processo=fork();
    if(processo==0){
        //exit(30);
        //setpgid(getpid(), 0);                                                               
        printf("-- Sono il figlio: %d\n",getpid()); 
        while(1);
    }

    waitpid(processo, &status, 0);
    
    switch(WTERMSIG(status)){
        case SIGUSR1:
            printf("Kill da SIGUSR1\n");
            break;
        default:
            printf("Ucciso da segnale non gestito %d\n", WTERMSIG(status));
            break;
    }

    /*if (WIFEXITED(status)) {
        printf("exited, status=%d\n", WEXITSTATUS(status));
    } else if (WIFSIGNALED(status)) {
        printf("killed by signal %d\n", WTERMSIG(status));
    } else if (WIFSTOPPED(status)) {
        printf("stopped by signal %d\n", WSTOPSIG(status));
    } else if (WIFCONTINUED(status)) {
        printf("continued\n");
    }*/

    return 0;
}