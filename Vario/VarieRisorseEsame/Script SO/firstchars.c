#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <fcntl.h>
#include <ctype.h>
#include <string.h>

#define MAXOPT 10

int main(int argc, char **argv){
    if(argc>10){
        system( " echo ?Too many arguments detected " );
        exit(2);
    }
    for(int i=1; i < argc; i++ ){
        int fd = open(argv[i],O_RDONLY);
        int fid = fork();
        char stringa[1];
        if(fid == 0){
            read(fd, stringa, 1);
            if(isalnum(stringa[0])==0){
                stringa[0]='-';
                fprintf(stderr, "%s", stringa);
            }else{
                fprintf(stdout, "%s", stringa);               
            }
            close(fd);
            exit(0);
        }
        wait(0);
    }
    fflush(stderr);
    sleep(1);
    fflush(stdout);

    printf("\n");
}
