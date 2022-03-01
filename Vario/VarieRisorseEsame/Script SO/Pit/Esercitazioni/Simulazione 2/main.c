/*
isdigit() ritorna != 0 se è un numero #include <ctype.h>
*/
#include <ctype.h>
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

#define WRITE 1
#define READ 0
#define MAX_FIGLI 5
#define DIM_COMANDO 100
int check_numero = 0;
int numero = 0;
char memoria[100];
int figli_attivi = 0;
int pid_figli[MAX_FIGLI];
pthread_t id_thread;
int num_thread = 0;
int fd[MAX_FIGLI][2];
struct sigaction sa;
int crea = 0;

void *funzioneThread(void *param)
{
    crea = 0;
    char base[25];
    char file[50];
    strcpy(base, "log");
    sprintf(file, "%s%d.txt", base, num_thread);
    int output = open(file, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    /**/
    char testo[150];
    sprintf(testo, "Thread iniziato con segnale %d\n", *(int *) param);
    write(output, testo, strlen(testo));
    for (int i = 0; i < figli_attivi; i++)
    {
        char msg[120];
        sprintf(msg, "[%d] %d %s", pid_figli[i], i, memoria);
        write(fd[i][WRITE], msg, sizeof(msg));
        close(fd[i][WRITE]);
    }
    write(output, "Inviati messaggi!\n", strlen("Inviati messaggi!\n"));
    figli_attivi = 0;
    crea=0;
    write(output, "Ripristinati figli attivi = 0!\n", strlen("Ripristinati figli attivi = 0!\n"));
    num_thread++;
    sleep(2);
    return (void *)3; //valore arbitrario
}

static void handler(int signo, siginfo_t *info, void *empty)
{
    pthread_create(&id_thread, NULL, funzioneThread, (void *)&signo);
}

int main(int argc, char **argv)
{
    printf("[PID] %d\n", getpid());

    while (1)
    {
        sleep(1); //glitch grafici
        char comando[DIM_COMANDO];
        //memset(comando, 0, DIM_COMANDO);
        printf("Dammi un comando: \n");
        fflush(stdout);
        scanf("%s", comando);
        int dimensione = strlen(comando);
        for (int i = 0; i < dimensione; i++)
        {
            if (isdigit(comando[i]) != 0)
                check_numero++;
        }

        if (check_numero == dimensione)
        {
            check_numero = 0;
            numero = 1;
            crea = 1;
        }
        else
        {
            numero = 0;
        }

        switch (numero)
        {
        case 0:
            strcpy(memoria, comando);
            //printf("Memoria: %s\n", memoria);
            break;
        case 1:
            //printf("Numero inserito: %d\n", atoi(comando));
            //--------------------------------------------------ERRORE!!!!!
            if (figli_attivi < 5 && crea == 1)
            {
                crea = 0;
                pipe(fd[figli_attivi]); //apertura PIPE
                int figlio = fork();
                if (figlio == 0)
                {
                    while (1)
                    {
                        //Sono nel figlio
                        char buffer[120];
                        close(fd[figli_attivi][WRITE]); //Chiudo lato scrittura figlio
                        read(fd[figli_attivi][READ], buffer, sizeof(buffer));
                        printf("%s\n", buffer);
                        sleep(1);
                        close(fd[figli_attivi][READ]);
                        crea=0;
                        exit(0);
                    }
                }
                else
                {
                    crea = 0;
                    close(fd[figli_attivi][READ]); //Chiudo lato lettura padre
                    pid_figli[figli_attivi] = figlio;
                    //system("ps");
                    sa.sa_sigaction = handler;     //utilizzo informazionia
                    sa.sa_flags |= SA_SIGINFO;     //flag per informazioni
                    sigaction(SIGUSR1, &sa, NULL); //segnale gestito in handler
                    sigaction(SIGUSR2, &sa, NULL);
                    printf("Pid figlio appena creato: %d\n", figlio);
                    figli_attivi++;
                }
            }
            else
            {
                //Non posso creare figlio
                crea = 0;
                printf("Massimo numero di figli raggiunto, SIGUSR1 o SIGUSR2 (a %d) per chiuderli tutti!\n", getpid());
            }
            break;
        default:
            fprintf(stderr, "Errore imprevisto!");
        }
    }
    return 0;
}
