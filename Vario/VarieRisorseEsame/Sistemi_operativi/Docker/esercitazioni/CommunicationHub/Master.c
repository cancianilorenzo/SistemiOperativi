/*
Le FIFO bloccano fino a quando non viene aperto l'altro lato!!
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

#define PATH "/tmp/fifo"

void inviaRicevi(char m, char n, char* testo)
{
    char path[10];
    char nomeFifo[100];
    strcpy(path, PATH);
    sprintf(nomeFifo, "%s%c", path,m);
    int fdScrittura = open(nomeFifo, O_WRONLY);
    char msg[110];
    sprintf(msg, "M%c-%s-", n,testo);
    if(write(fdScrittura, msg, sizeof(msg))==-1) printf("Errore nell'invio\n");
    close(fdScrittura);
}

int main(int argc, char **argv)
{
    char path[10];
    strcpy(path, PATH);

    int slaves = atoi(argv[1]);

    //==========================================================
    //TEST SOLO 1 FIFO PER SLAVE
    for (int i = 1; i < slaves+1; i++)
    {
        char pipeName[100];
        sprintf(pipeName, "%s%d", path, i);
        mkfifo(pipeName, S_IRUSR | S_IWUSR); //FUNZIONA
        //printf("%s\n", pipeName);
    }

    //==========================================================

    for (int i = 1; i < slaves + 1; i++)
    {
        int processo = fork();
        if (processo == 0)
        {
            char idProcesso[2];
            sprintf(idProcesso, "%d", i);
            char *argexec[] = {idProcesso, argv[1], NULL};
            execvp("./slave.out", argexec);
        }
    }

    while (1)
    {
        char comando[3];
        char messaggio[100];
        sleep(1);
        printf("Dammi un comando: ");
        fflush(stdout);
        scanf("%s", comando);
        scanf("%s", messaggio);

        char verso = comando[1];
        if (verso == '>')
        {
            inviaRicevi(comando[0], comando[2], messaggio);
        }
        else if (verso == '<')
        {
            inviaRicevi(comando[2], comando[0], messaggio);
        }
        else
        {
            printf("Segno non accettato!!\n");
        }
    }

    return 0;
}