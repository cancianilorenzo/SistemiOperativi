#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <pthread.h>
#include <sys/wait.h>

/*
File + intero
*/

int numProc;
int fogliePid[10];
char pid[8];
int processiAttivi = 0;
int pidSorgente;

struct sigaction sig, sigF;

static void handler(int signo, siginfo_t *info, void *ctx)
{
    pidSorgente = (int)info->si_pid; //pid del processo che ha inviato il segnale
    if (processiAttivi == 1)
    {
        kill(fogliePid[processiAttivi], SIGQUIT);
        processiAttivi = 0;
        while (wait(NULL) > 0)
            ;
        exit(0);
    }
    else
    {
        kill(fogliePid[processiAttivi], SIGQUIT);
        processiAttivi--;
    }
}

void setHandler()
{
    sig.sa_flags = SA_SIGINFO;
    sig.sa_sigaction = &handler;
    sigaction(SIGUSR1, &sig, 0);
}

static void handlerFoglia(int signo, siginfo_t *info, void *ctx)
{
    kill(pidSorgente,SIGUSR2);
    exit(0);
    
}

void setHandlerFoglia()
{
    sigF.sa_sigaction = &handlerFoglia;
    sigaction(SIGQUIT, &sigF, 0);
}

int main(int argc, char **argv)
{

    if (argc != 3)
    {
        return 3; //Termina se != 3
    }
    //printf("TEST\n");

    numProc = atoi(argv[2]);
    //printf("%i\n", numProc);
    if (numProc < 1 || numProc > 10)
    {
        //printf("OUT OF RANGE\n");
        return 4;
    }
    processiAttivi = numProc;

    int test = open(argv[1], S_IRUSR | S_IWUSR);
    if (test > 0)
    {
        //printf("ESISTE GIA\n");
        return 5; //Esce se problemi con il file
    }
    //--------------------------------------------GEST ERR OK

    sprintf(pid, "%i\n", getpid());
    //printf("%s\n",pid); fflush(stdout);
    int file = open(argv[1], O_CREAT | O_RDWR, 0777);
    write(file, pid, strlen(pid));
    sleep(1);
    int manager = fork();
    if (manager == 0)
    {
        memset(&sig,0,sizeof(sig));
        setHandler();

        for (int i = 0; i < numProc; i++)
        {
            int foglia = fork();
            if (foglia == 0)
            {
                memset(&sig,0,sizeof(sig));
                setHandlerFoglia();
                while (1)
                    ;
            }
            else
            {
                fogliePid[i] = foglia;
                sprintf(pid, "%i\n", foglia);
                write(file, pid, strlen(pid));
            }
        }
        /*for (int i = 0; i < numProc; i++){
            printf("%i\n",fogliePid[i]);
        }*/
        while (1)
            ;
    }
    else
    {
        sprintf(pid, "%i\n", manager);
        write(file, pid, strlen(pid));
    }

    return 0;
}