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

#define PATH_MAXLEN 60
#define PIDLEN 8
int scrivi_figlio = 0;
int fd_figlio;
int id_coda;

/*
path + numero 1-10
*/

struct messaggio
{
    long mtype;         //tipo messaggio, identifica la classe
    char mtext[PIDLEN]; //campo testuale
} messaggio, ricevuto;


static void handler(int signo, siginfo_t *info, void *empty)
{
    switch (signo)
    {
    case SIGUSR1:
        write(fd_figlio, "SIGUSR1\n", strlen("SIGUSR1\n"));
        scrivi_figlio = 1;
        break;
    case SIGUSR2:
        messaggio.mtype = 1;
        char coda_msg_figlio[PIDLEN];
        sprintf(coda_msg_figlio, "%d", getpid());
        strcpy(messaggio.mtext, coda_msg_figlio);
        int esito = msgsnd(id_coda, &messaggio, sizeof(messaggio.mtext), 0);
        scrivi_figlio = 2;
        break;
    }
}

void quit(int err)
{
    char msg[100];
    switch (err)
    {
    case 0:
        strcpy(msg, "\n");
        break;
    case 2:
        strcpy(msg, "?Wrong number of arguments\n");
        break;
    case 3:
        strcpy(msg, "?Wrong argument 'n'\n");
        break;
    case 4:
        strcpy(msg, "?'path' too long\n");
        break;
    case 5:
        strcpy(msg, "?Error in path\n");
        break;
    case 6:
        strcpy(msg, "?Fork error\n");
        break;
    default:
        strcpy(msg, "?Generic error\n");
        break;
    };
    if (err > 0)
    { //errori su stderr
        fprintf(stderr, "%s", msg);
    }
    else
    { //successo su stdout
        printf("%s\n", msg);
    };
    fflush(stderr);
    fflush(stdout);
    exit(err);
};

int main(int argc, char **argv)
{
    system("rm -r tmp/info");
    system("clear");

    if (argc != 3)
        quit(2);
    if (atoi(argv[2]) > 10 || atoi(argv[2]) < 1)
        quit(3);
    if (strlen(argv[1]) > PATH_MAXLEN)
        quit(4);
    char path[PATH_MAXLEN];
    strcpy(path, argv[1]);
    int numero = atoi(argv[2]);

    char percorso_totale[100];

    sprintf(percorso_totale, "%s/info", path);
    if (mkdir(percorso_totale, 0755) != 0)
        quit(5);
    char file_key[110];
    sprintf(file_key, "%s/key.txt", percorso_totale);
    int fd_padre = open(file_key, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    char pid[PIDLEN + 2];
    sprintf(pid, "%d\n", getpid());
    write(fd_padre, pid, strlen(pid));
    close(fd_padre);

    key_t coda_key = ftok(file_key, 32);
    id_coda = msgget(coda_key, 0777 | IPC_CREAT);
    msgctl(id_coda, IPC_RMID, NULL);
    id_coda = msgget(coda_key, 0777 | IPC_CREAT);
    messaggio.mtype = 1;
    char coda_msg[PIDLEN];
    sprintf(coda_msg, "%d", getpid());
    strcpy(messaggio.mtext, coda_msg);
    int esito = msgsnd(id_coda, &messaggio, sizeof(messaggio.mtext), 0);

    for (int i = 0; i < numero; i++)
    {
        int processo = fork();
        if (processo == -1)
            quit(6);
        if (processo == 0)
        {
            struct sigaction sa;           //struct sigaction
            sa.sa_sigaction = handler;     //utilizzo informazioni
            sa.sa_flags |= SA_SIGINFO;     //flag per informazioni
            sigaction(SIGUSR1, &sa, NULL); //segnale gestito in handler
            sigaction(SIGUSR2, &sa, NULL);

            //Sono nel figlio
            char file_pid[110];
            sprintf(file_pid, "%s/%d.txt", percorso_totale, getpid());
            fd_figlio = open(file_pid, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
            while(1);
        }
        else
        {
            printf("%d ", processo);
        }
    }
    printf("\n");
    fflush(stdout);

    return 0;
}