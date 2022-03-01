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

int main(int argc, char** argv){
    printf("[PID PADRE] %i\n", getpid());
    sigset_t mod, old;
    //sigemptyset(&mod); //Svuota la maschera
    //sigemptyset(&old);
    sigfillset(&old); //Contiene tutti i segnali
    sigdelset(&mod, SIGKILL); //rimuove SIGKILL
    sigprocmask(SIG_BLOCK, &mod, &old);
    /*int figlio = fork();
    if (figlio != 0){
        //sono nel padre
        
    }*/
    while(1);

    return 0;
}