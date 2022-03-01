#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <fcntl.h>

#define MAXOPT 10

char check(char stringa){
    char c = '-';
    if (stringa >= 'a' && stringa <= 'z') c = stringa;
    if (stringa >= 'A' && stringa <= 'Z') c = stringa;
    return c;
}


int main(int argc, char **argv){
    
    //argomenti
    if(argc>10){ system( " echo ?Too many arguments detected " ); exit(2); }

    printf("[PID-FATHER] is: %d\n", getpid());

    int n_files=argc-1;
    int array_pid[n_files];
    int f;

    for(int i=0; i < n_files; i++ ){
        f=fork();
        if(f==0){
            printf("-- [PID-%d] is: %d\n", i, getpid());
            int fd = open(argv[i+1],O_RDONLY);
            char stringa[1];
            read(fd, &stringa, sizeof(stringa));
            char c = check(stringa[0]);
            printf("Read: %c\n", c);
            exit(0);
        }else{
            array_pid[i]=getpid();
        }
    }
    wait(wait(NULL)>0);  
    return 0;
}
