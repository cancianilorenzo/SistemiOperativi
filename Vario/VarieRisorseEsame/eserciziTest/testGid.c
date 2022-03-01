/*
Creo figlio, setto group leader
creo sottofigli
mando kill da padre a tutto il gruppo
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
#include <ctype.h>

int main(int argc, char **argv)
{
    int gid;

    int processo = fork();
    if (processo == 0)
    {
        setpgid(getpid(), getpid());
        for (int i = 0; i < 5; i++)
        {
            int figlio = fork();
            {
                if (figlio == 0)
                {
                    while (1);
                }
                else{
                    setpgid(figlio,getpid());
                }
            }
        }
    }
    else
    {
        gid = processo;
        printf("GID --> %d\n", gid);
    }

    while(wait(NULL)>0);

    return 0;
}