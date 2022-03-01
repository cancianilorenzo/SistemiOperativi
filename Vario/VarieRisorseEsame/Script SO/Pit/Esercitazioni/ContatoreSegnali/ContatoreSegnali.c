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

#define MITTENTI 5

int mittenti[MITTENTI];
int ricevutiSIG1[MITTENTI];
int ricevutiSIG2[MITTENTI];
int attuale = 0;
int gestito;

bool foreground()
{
    bool fg = false;
    if (getpgrp() == tcgetpgrp(STDOUT_FILENO))
        fg = true;
    return fg;
}

static void handler(int signo, siginfo_t *info, void *empty)
{
    gestito = 0;

    switch (signo)
    {
    case SIGUSR1:
        //printf("SIGUSR1 [PID]: %i\n", info->si_pid);
        for (int i = 0; i < MITTENTI; i++)
        {
            if (mittenti[i] == info->si_pid)
            {
                ricevutiSIG1[i]++;
                gestito = 1;
                if (foreground() == true)
                {
                    printf("Aggiunto SIG1 a processo in pos %d da [PID] %d\n", i, info->si_pid);
                }
            }
        }
        if (gestito == 0 && attuale < MITTENTI)
        {
            //Ho ancora spazio nei mittenti
            mittenti[attuale] = info->si_pid;
            ricevutiSIG1[attuale]++;
            gestito = 1;
            if (foreground() == true)
                {
                    printf("Aggiunto SIG1 a processo in pos %d da [PID] %d\n", attuale, info->si_pid);
                }
            attuale++;
        }
        break;

    case SIGUSR2:
        //printf("SIGUSR2 [PID]: %i\n", info->si_pid);
        for (int i = 0; i < MITTENTI; i++)
        {
            if (mittenti[i] == info->si_pid)
            {
                ricevutiSIG2[i]++;
                gestito = 1;
                if (foreground() == true)
                {
                    printf("Aggiunto SIG2 a processo in pos %d da [PID] %d\n", i, info->si_pid);
                }
            }
        }
        if (gestito == 0 && attuale < MITTENTI)
        {
            //Ho ancora spazio nei mittenti
            mittenti[attuale] = info->si_pid;
            ricevutiSIG2[attuale]++;
            gestito = 1;
            if (foreground() == true)
                {
                    printf("Aggiunto SIG2 a processo in pos %d da [PID] %d\n", attuale, info->si_pid);
                }
            attuale++;
        }
        break;

    case SIGINT:
        for (int i = 0; i < attuale - 1; i++)
        {
            printf("Da processo %d, %d-SIG1, %d-SIG2\n", mittenti[i], ricevutiSIG1[i], ricevutiSIG2[i]);
        }
        exit(0);
        break;
    case SIGTERM:
        for (int i = 0; i < attuale; i++)
        {
            printf("Da processo %d, %d-SIG1, %d-SIG2\n", mittenti[i], ricevutiSIG1[i], ricevutiSIG2[i]);
        }
        exit(0);
        break;
    default:
        printf("Segnale non gestito!\n");
    }
}

int main(int argc, char **argv)
{
    struct sigaction sa; //struct sigaction
    printf("[PID] %i\n", getpid());
    fflush(stdout);
    sleep(2);

    while (1)
    {
        sa.sa_sigaction = handler;     //utilizzo informazioni
        sa.sa_flags |= SA_SIGINFO;     //flag per informazioni
        sigaction(SIGUSR1, &sa, NULL); //segnale gestito in handler
        sigaction(SIGUSR2, &sa, NULL);
        sigaction(SIGINT, &sa, NULL);
        sigaction(SIGTERM, &sa, NULL);
    };

    return 0;
}
