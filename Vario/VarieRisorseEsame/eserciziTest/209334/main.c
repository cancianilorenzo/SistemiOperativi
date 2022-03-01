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
#include <termios.h>

#define MAX_CHILD 10
int figli = 0;
int file;
int crea = 0;
int pid_figli[MAX_CHILD];
char msg[2];
int sigusr1 = 0;

void quit(int err)
{
    char msg[100];
    switch (err)
    {
    case 0:
        strcpy(msg, "");
        break;
    case 2:
        strcpy(msg, "?Wrong number of arguments\n");
        break;
    case 3:
        strcpy(msg, "?Wrong argument 'server' or 'client'\n");
        break;
    case 4:
        strcpy(msg, "?Error in file opening\n");
        break;
    default:
        strcpy(msg, "?Generic error");
        break;
    };
    if (err > 0)
    { //errori su stderr
        fprintf(stderr, "%s", msg);
    }
    else
    { //successo su stdout
        printf("%s", msg);
    };
    fflush(stderr);
    fflush(stdout);
    exit(err);
};

typedef enum
{
    KP_ECHO_OFF,
    KP_ECHO_ON,
} kp_echo_t;
int keypress(const kp_echo_t echo)
{
    struct termios savedState, newState;
    unsigned char echo_bit;
    int c;
    if (-1 == tcgetattr(STDIN_FILENO, &savedState))
    {
        return EOF;
    };
    newState = savedState;
    if (KP_ECHO_OFF == echo)
    {
        echo_bit = ECHO;
    }
    else
    {
        echo_bit = 0;
    };
    /* canonical input + set echo with minimal input as 1. */
    newState.c_lflag &= ~(echo_bit | ICANON);
    newState.c_cc[VMIN] = 1;
    if (-1 == tcsetattr(STDIN_FILENO, TCSANOW, &newState))
    {
        return EOF;
    };
    c = getchar(); /* block until key press */
    if (-1 == tcsetattr(STDIN_FILENO, TCSANOW, &savedState))
    {
        return EOF;
    };
    return c;
}

static void handler(int signo, siginfo_t *info, void *empty)
{
    switch (signo)
    {
    case SIGUSR1:
        //printf("SIGUSR1 da sigaction da [PID]: %i\n", info->si_pid);
        crea = 1;
        //Nuovo figlio + pid su file + '+'
        break;
    case SIGUSR2:
        //printf("SIGUSR2 da sigaction da [PID]: %i\n", info->si_pid);
        if (figli > 0)
        {
            kill(pid_figli[figli - 1], SIGTERM);
            char msg_kill[10]; //pid + \n + '+'
            sprintf(msg_kill, "-%d\n", pid_figli[figli - 1]);
            write(file, msg_kill, strlen(msg_kill));
            printf("[server] %s", msg_kill);
            //printf("Ucciso figlio %d\n", figli - 1);
            figli--;
        }
        else
        {
            write(file, "0\n", strlen("0\n"));
            printf("[server] 0\n");
        }
        break;

    case SIGINT:
        //printf("SIGINT da sigaction da [PID]: %i\n", info->si_pid);
        sprintf(msg, "%d", figli);
        write(file, msg, sizeof(msg));
        quit(0);
        break;
    }
}

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        quit(2);
    }
    if (strcmp(argv[1], "server") == 0)
    {
        //Sono nel server
        //printf("Sono nel server\n");
        printf("[server:%d]\n", getpid());
        file = open(argv[2], O_CREAT | O_RDWR | O_EXCL, S_IRUSR | S_IWUSR);
        if (file < 0)
        {
            quit(4);
        }
        char msg[sizeof(getpid() + 2)];
        sprintf(msg, "%d", getpid());
        //printf("%s\n", msg);
        write(file, msg, sizeof(msg));
        write(file, "\n", strlen("\n"));

        //printf("File aperto\n");
        while (1)
        {
            struct sigaction sa;           //struct sigaction
            sa.sa_sigaction = handler;     //utilizzo informazioni
            sa.sa_flags |= SA_SIGINFO;     //flag per informazioni
            sigaction(SIGUSR1, &sa, NULL); //segnale gestito in handler
            sigaction(SIGUSR2, &sa, NULL);
            sigaction(SIGINT, &sa, NULL);
            if (crea == 1)
            {
                if (figli < MAX_CHILD)
                {
                    //creo figlio
                    int processo = fork();
                    if (processo == 0)
                    {
                        //printf("Creato figlio con pid %d\n", getpid());
                        while (1)
                            ;
                    }
                    else
                    {
                        pid_figli[figli] = processo;
                        char msg[6]; //pid + \n + '+'
                        sprintf(msg, "+%d\n", processo);
                        write(file, msg, strlen(msg));
                        printf("[server] %s", msg);
                        figli++;
                        crea = 0;
                    }
                }
                else
                {
                    //printf("Troppi figli!\n");
                    crea = 0;
                    break;
                }
            }
        }
    }

    else if (strcmp(argv[1], "client") == 0)
    {
        file = open(argv[2], O_RDWR, S_IRUSR | S_IWUSR);
        while (file < 0)
        {
            file = open(argv[2], O_RDWR, S_IRUSR | S_IWUSR); //ciclo fino all'apertura
        }
        lseek(file, 0, SEEK_SET);
        char buffer[1];
        int pid_len = 0;
        int l = read(file, &buffer, 1);
        while (buffer[0] != '\n')
        {
            pid_len++;
            l = read(file, &buffer, 1);
        }
        close(file); //equivale reset testina lseek(file, 0, SEEK_SET)
        file = open(argv[2], O_RDWR, S_IRUSR | S_IWUSR);
        char pid_padre[sizeof(getpid())];
        read(file, &pid_padre, sizeof(getpid()));
        printf("[client] server:%s\n", pid_padre);

        char c;
        while (1)
        {
            c = keypress(KP_ECHO_OFF); // read single keypress without echoing
            if (c == '+')
            {
                if (figli < MAX_CHILD)
                {
                    kill(atoi(pid_padre), SIGUSR1);
                    sigusr1++;
                    figli++;
                    printf("[client] %d\n", sigusr1);
                }
            };
            if (c == '-')
            {
                if (figli > 0)
                {
                    kill(atoi(pid_padre), SIGUSR2);
                    sigusr1--;
                    figli--;
                    printf("[client] %d\n", sigusr1);
                }
            };
            if (c == '\n')
            {
                for (int i = 0; i < sigusr1; i++)
                {
                    kill(atoi(pid_padre), SIGUSR2);
                    sleep(1); //1 secondo
                }
                kill(atoi(pid_padre), SIGINT);
                printf("[client] %d\n", sigusr1);
                quit(0);
                break;
            };
        };
    }

    else
    {
        quit(3);
    }

    return 0;
}
