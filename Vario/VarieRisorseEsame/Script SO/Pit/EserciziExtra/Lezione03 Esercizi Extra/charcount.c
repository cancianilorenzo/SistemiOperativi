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

    if(argc != 2){
        fprintf(stderr,"Inserire il carattere di cui contare le occorrenze!!\n");
    }

    char c[1];
    int letto = read(STDIN_FILENO, c, 1);
    while(letto != 0){
        printf("%s", c[0]);
        letto = read(STDIN_FILENO, c, 1);
    }

    return 0;
}