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

extern int errno;

int main(int argc, char** argv){
    int fd = open("filenonesistente.txt", O_RDWR, S_IRUSR|S_IWUSR);
    if(fd < 0){
        perror("Error printed by perror");
    }
    return 0;
}