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

#define LETTURA 0
#define SCRITTURA 1

int main(int argc, char **argv){

    int fd_1[2]; //figlio legge
    int fd_2[2]; //padre legge
    printf("[PADRE] %d\n", getpid());

    pipe(fd_1); pipe(fd_2);
    close(fd_1[LETTURA]);
    close(fd_2[SCRITTURA]);

    int processo = fork();

    if(processo==0){
        int i = 0;
        close(fd_2[LETTURA]);
        close(fd_1[SCRITTURA]);
        while(1){
            if(i==5) write(fd_2[SCRITTURA], "quit", strlen("quit"));
            char buffer[100];
            write(fd_2[SCRITTURA], "Hello", strlen("Hello"));
            read(fd_1[LETTURA], &buffer, 100);
            if(strcmp("quit", buffer)==0) break;
            printf("[figlio] Ricevuto: %s\n", buffer);
            i++;
        }
    }else{

        while(1){
            char buffer[100];
            read(fd_2[LETTURA], &buffer, 100);
            if(strcmp("quit", buffer)==0) break;
            printf("[padre] Ricevuto: %s\n", buffer);
            write(fd_1[SCRITTURA], "Ciao", strlen("Ciao"));
        }
        write(fd_1[SCRITTURA], "quit", strlen("quit"));
    }

return 0;
}