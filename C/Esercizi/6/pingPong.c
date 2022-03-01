/*
Pipe anonime
*/

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

#define READ 0
#define WRITE 1

int main(int argc, char **argv)
{

    int fd_padre[2];
    int fd_figlio[2];
    pipe(fd_padre);
    pipe(fd_figlio);

    int figlio = fork();

    if (figlio == 0)
    {
        close(fd_padre[WRITE]);
        close(fd_figlio[READ]);
        //sono nel figlio
        while (1)
        {
            char buffer[100];
            read(fd_padre[READ], buffer, sizeof(buffer));
            if (strcmp(buffer, "quit") == 0)
            {
                printf("Ho ricevuto quit, termino!\n");
                exit(0);
            }
            write(fd_figlio[WRITE], buffer, strlen(buffer));
        }
    }
    else
    {
        close(fd_padre[READ]);
        close(fd_figlio[WRITE]);
        while (1)
        {
            sleep(1); //glitch grafici nei print
            char buffer[100];
            memset(buffer, 0, 100);
            printf("Comando: ");
            fflush(stdout);
            scanf("%s", buffer);
            if (strcmp(buffer, "quit") == 0)
            {
                write(fd_padre[WRITE], buffer, strlen(buffer));
                while (wait(NULL) > 0);
                sleep(1);
                printf("Figlio terminato!\n");
                exit(0);
            }
            else
            {
                write(fd_padre[WRITE], buffer, strlen(buffer));
                read(fd_figlio[READ], buffer, sizeof(buffer));
            }
        }
    }

    return 0;
}