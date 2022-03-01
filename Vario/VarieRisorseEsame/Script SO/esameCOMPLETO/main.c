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

int processoMittente;
int fd[2];

static void handler(int signo, siginfo_t *info, void *empty)
{
    if (signo == SIGUSR1)
    {
        processoMittente = info->si_pid;
        printf("ProcessoMittente: %i\n", processoMittente);
        write(fd[WRITE], "1", 1);
    }

    if (signo == SIGTERM)
    {
        //Scrivo 10 volte sulla pipe, invio a 10 processi, resterà qualcosa in pending se n<10
        for (int j = 0; j < 10; j++)
        {
            write(fd[WRITE][j], "1", 1);
        }
    }
}


int main(int argc, char **argv)
{

    if (argc != 3)
    {
        fprintf(stderr, "[ERR]: Usage ./a.out <file target> <n>\n");
        return 3;
    }

    int n = atoi(argv[2]);

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

    int manager = fork();

    if (manager != 0)
    {
        //Sono nel padre
        char buf[10];
        sprintf(buf, "%i\n", getpid());
        write(fileTarget, buf, sizeof(getpid()) + 1);
    }

    sleep(1);

    if (manager == 0)
    {
        pipe(fd);

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
                close(fd[WRITE]);
                setpgid(getpid(),pidManager);

                char buffer[1];
                read(fd[READ], &buffer, 1);
                printf("Processo %i, buffer %c\n", i,buffer[0]);
                kill(processoMittente, SIGUSR2);
                return 0;
            }
        }
        close(fd[READ]);

        struct sigaction sa;
        sa.sa_sigaction = handler;
        sigemptyset(&sa.sa_mask);
        sa.sa_flags |= SA_SIGINFO;
        sigaction(SIGUSR1, &sa, NULL);


        while (wait(NULL) > 0)
            ;
        close(fd[WRITE]);
    }

    return 0;
}