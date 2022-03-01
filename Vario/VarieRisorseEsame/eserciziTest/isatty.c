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

void main(int argc, char **argv)
{

    if (isatty(0))
        printf("the standard input is from a terminal\n");
    else
        printf("the standard input is NOT from a terminal\n");
    if (isatty(1))
        printf("the standard output is to a terminal\n");
    else
        printf("the standard output is NOT to a terminal\n");
    exit(0);
}