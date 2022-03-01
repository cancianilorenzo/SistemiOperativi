#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

//./eseguibile <comando1> <comando2>
//equivale a ls | wc

int main(int argc, char **argv){
    char *args;
    int fd = open("a.txt", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    int fid=fork();
    if(fid==0){
        sprintf(args, "/usr/bin/%s", argv[1]);
        dup2(fd, 1);
        execl(args, args, NULL);
    }
    if(fid!=0){
        wait(&fid);
        sprintf(args, "/usr/bin/%s", argv[2]);
        lseek(fd, 0, SEEK_SET);
        dup2(fd, 0);
        execl(args, args, NULL);
    }
    close(fd);
}