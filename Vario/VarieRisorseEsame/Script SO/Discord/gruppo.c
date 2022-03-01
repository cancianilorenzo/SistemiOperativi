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

int pidGruppo;

int main(int argc, char **argv)
{

    int pidPadre = getpid();
    setpgid(pidPadre, 0);
    int figlio = fork();

    if (figlio == 0)
    {
        //sono nel figlio
        setpgid(getpid(), 0);
        while(1);
    }
    else
    {
        pidGruppo = figlio;
    }

    int figlio1 = fork();

    if (figlio == 0)
    {
        //sono nel figlio
        setpgid(getpid(), pidGruppo);
        while(1);
    }

    return 0;
}