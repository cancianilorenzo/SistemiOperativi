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
#include <ctype.h>

int main(int argc, char **argv)
{
    if (argc > 11)
    {
        fprintf(stderr, "Troppi argomenti! Massimo 10!\n");
        exit(1);
    }

    for (int i = 1; i < argc; i++)
    {
        int figlio = fork();
        if (figlio == 0)
        {
            int fd = open(argv[i], O_RDWR, S_IRUSR | S_IWUSR);
            char c[1];
            read(fd, c, 1);
            if (isalnum(c[0]))
            {
                printf("%c", c[0]);
            }
            else
            {
                fprintf(stderr, "?");
            }
            exit(0);
        }

    }
    while(wait(NULL)>0);
    fflush(stdout);
    fflush(stderr);
    printf("\n");

    return 0;
}