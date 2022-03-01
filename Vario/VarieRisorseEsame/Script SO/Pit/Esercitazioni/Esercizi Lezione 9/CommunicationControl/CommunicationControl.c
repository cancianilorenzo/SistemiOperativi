/*
Pipe anonime
N figli --> 2N pipe (1M, M1)
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
#include <time.h>

#define SCRITTURA 1
#define LETTURA 0

void main(int argc, char **argv)
{
    int figli = atoi(argv[1]);
    int fd_master[figli][2];
    int fd_figlio[figli][2];

    for (int i = 0; i < figli; i++)
    {
        int esito_master = pipe(fd_master[i]);
        int esito_figli = pipe(fd_figlio[i]);
        int processo = fork();
        if (processo == 0)
        {
            //Son nel figlio
            close(fd_master[i][SCRITTURA]);
            close(fd_figlio[i][LETTURA]);
            while (1)
            {
                char buffer[1];
                read(fd_master[i][LETTURA], &buffer, sizeof(buffer));
                if (buffer[0] == 'i')
                {
                    printf("scrivo pid\n");
                    char pid[sizeof(getpid()) + 1];
                    sprintf(pid, "%d", getpid());
                    if (write(fd_figlio[i][SCRITTURA], pid, strlen(pid)) == -1)
                        printf("Errore scrittura 2\n");
                }

                if (buffer[0] == 'r')
                {
                    srand(time(NULL));
                    printf("scrivo random\n");
                    char pid[10];
                    sprintf(pid, "%d", rand());
                    if (write(fd_figlio[i][SCRITTURA], pid, strlen(pid)) == -1)
                        printf("Errore scrittura 2\n");
                }
                if (buffer[0] == 'q')
                {
                    exit(0);
                }
            }
        }
        else
        {
            //Sono nel padre
            close(fd_master[i][LETTURA]);
            close(fd_figlio[i][SCRITTURA]);
        }
    }

    while (1)
    {
        int richiesto = 0;
        int figlio;
        sleep(1);
        printf("Comando: ");
        fflush(stdout);

        char buffer[100];
        scanf("%s", buffer);

        if (buffer[0] == 'i' || buffer[0] == 'r')
        {
            char figlio_char[strlen(buffer)];

            for (int i = 0; i < strlen(buffer); i++)
            {
                figlio_char[i] = buffer[i + 1];
            }
            figlio = atoi(figlio_char);
            if(figlio <= figli){
            write(fd_master[figlio - 1][SCRITTURA], &buffer[0], 1);

            char ingresso[100];
            read(fd_figlio[figlio - 1][LETTURA], ingresso, sizeof(ingresso));
            printf("Messaggio dal figlio --> %s\n", ingresso);
            }
            else{
                printf("Out of range!\n");
            }
        }
        if (buffer[0] == 'q')
        {
            for (int i = 0; i < figli; i++)
            {
                write(fd_master[i][SCRITTURA], &buffer[0], 1);
            }
            printf("Terminazione di tutti i figli in corso....");
            while (wait(NULL) > 0);
            printf("Completeto!\n");
            exit(0);
        }
    }
}