#include <stdio.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <unistd.h>
void main(int argc, char** argv)
{
    char *args[2];
    int tmpFile = open("temp.txt", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    int f = fork();
    if (f == 0)
    {
        //args[0] = "/usr/bin/ls";
        sprintf(args[0], "/usr/bin/%s", argv[1]);
        args[1] = NULL;
        dup2(tmpFile, 1);
        execvp(args[0], args);  
    }
    else
    {
        wait(NULL);
        sprintf(args[0], "/usr/bin/%s", argv[2]);
        args[1] = NULL;
        dup2(tmpFile, 0);
        lseek(tmpFile, 0, SEEK_SET);
        execvp(args[0], args);
    };
    close(tmpFile);
}