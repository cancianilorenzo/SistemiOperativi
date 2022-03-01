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
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdbool.h>

int main(int argc, char** argv){
    if(argc != 3){
        printf("Errore nell'uso dell'eseguibile!\n");
        exit(1);
    }

    for(int i = 0; i < 2; i++){
        int figlio = fork();
        if(figlio == 0){
            char pid[sizeof(getpid())];
            sprintf(pid, "%d", getpid());
            char *args[] = {pid, NULL};
            execv(argv[i+1], args);
        }
    }

    return 0;
}