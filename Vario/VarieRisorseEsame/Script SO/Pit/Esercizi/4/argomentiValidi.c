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

#define MAX_ARG 10

int main(int argc, char **argv)
{
    char memoria[argc];
    char opt;
    int i = 1;
    while (i < argc)
    {
        opt = argv[i][0];
        switch (opt)
        {
        case 'c':
            memoria[i-1] = 'c';
            i++;
            break;

        case 'l':
            memoria[i-1] = 'l';
            i++;
            break;

        default:
            memoria[i-1] = '?';
            i++;
        }
    }
    printf("Memoria --> %s\n", memoria);
    return 0;
}