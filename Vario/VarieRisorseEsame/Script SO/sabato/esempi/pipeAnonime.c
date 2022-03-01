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

int main(int argc, char** argv)
{
    int fd[2];
    int esito = pipe(fd);

    if(esito == 0){
        //pipe create correttamente
        write(fd[1],"writing",strlen("writing"));
        char buf[50];
        int c = read(fd[0], &buf, 50);
        printf("%s\n", buf);
    }
}