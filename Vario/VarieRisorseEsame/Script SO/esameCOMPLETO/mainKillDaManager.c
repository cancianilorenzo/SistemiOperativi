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

#define WRITE 1
#define READ 0

/*
Due argomenti
nome file --> target
numero processi --> n (tra 1 e 10)
*/

int processoMittente, pidFoglie[10], n, totali;
struct sigaction sa, sa_foglie;

//Definizione handler manager
//===================================================================================================
static void handler_manager(int signo, siginfo_t *info, void *empty)
{
    if (signo == SIGUSR1)
    {
        processoMittente = info->si_pid;
        //printf("ProcessoMittente: %i\n", processoMittente);
        kill(pidFoglie[totali - 1], SIGUSR2);
        //printf("TOTALI-1 --> %i\n", totali - 1);
        //printf("kill foglia %i\n", totali - 1);
        totali--;
        //printf("TOTALI --> %i\n", totali);
    }

    if (signo == SIGTERM)
    {
        for (int j = 0; j < n; j++)
        {
            kill(pidFoglie[j], SIGTERM);
        }
    }
}

//Definizione handler foglie
//===================================================================================================
static void handler_foglie(int signo, siginfo_t *info, void *empty)
{
    if (signo == SIGUSR1)
    {
        kill(processoMittente, SIGUSR2);
        exit(0);
    }
}

int main(int argc, char **argv)
{

    //Controllo argomenti
    //===================================================================================================

    if (argc != 3)
    {
        fprintf(stderr, "[ERR]: Usage ./a.out <file target> <n>\n");
        return 3;
    }

    n = atoi(argv[2]);
    totali = n;
    if (n > 10 || n < 1)
    {
        fprintf(stderr, "[ERR]: <n> out of range <1...10>\n");
        return 4;
    }

    int fileTarget = open(argv[1], O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    if (fileTarget < 0)
    {
        fprintf(stderr, "[ERR]: File doesn't exist or the path is incorrect\n");
        return 5;
    }

    //Creo manager
    //===================================================================================================

    int manager = fork();

    if (manager != 0)
    {
        //Sono nel padre
        char buf[10];
        sprintf(buf, "%i\n", getpid());
        write(fileTarget, buf, sizeof(getpid()) + 1);
    }

    sleep(1);

    //Creo foglie
    //===================================================================================================

    if (manager == 0)
    {
        //Sono nel manager, creo n foglie
        char buf[10];
        sprintf(buf, "%i\n\n", getpid());
        write(fileTarget, buf, sizeof(getpid()) + 1);
        int pidManager = getpid();

        for (int i = 0; i < n; i++)
        {
            int foglia = fork();
            if (foglia == 0)
            {
                sprintf(buf, "%i\n", getpid());
                write(fileTarget, buf, sizeof(getpid()) + 1);

                sa_foglie.sa_sigaction = handler_foglie;
                sigemptyset(&sa_foglie.sa_mask);
                sa_foglie.sa_flags |= SA_SIGINFO;
                sigaction(SIGUSR2, &sa_foglie, NULL);
                while (1)

                    ;
            }
            if (foglia != 0)
            {
                pidFoglie[i] = foglia;
                for (int h = 0; h < n; h++)
                {
                    printf("%i, %i\n", h, pidFoglie[h]);
                }
            }
        }

        while (1)
        {
            sa.sa_sigaction = handler_manager;
            sigemptyset(&sa.sa_mask);
            sa.sa_flags |= SA_SIGINFO;
            sigaction(SIGUSR1, &sa, NULL);
            sigaction(SIGTERM, &sa, NULL);
        }
    }

    return 0;
}