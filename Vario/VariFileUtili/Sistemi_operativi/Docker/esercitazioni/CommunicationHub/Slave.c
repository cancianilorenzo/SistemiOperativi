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
char stringa[100];

char *parsing(char *fifoContent)
{
    memset(stringa, 0, 100);

    int delimitatori[2];
    int attuale = 0;
    for (int i = 0; i < strlen(fifoContent); i++)
    {
        if (fifoContent[i] == '-')
        {
            delimitatori[attuale] = i;
            attuale++;
        }
    }

    for (int j = 0; j < delimitatori[1] - 1; j++)
    {
        stringa[j] = fifoContent[delimitatori[0] + 1 + j];
    }

    return stringa;
}

void inviaComando(char *fifoContent, int id)
{
    char path[10];
    char nomeFifo[100];
    strcpy(path, PATH);
    sprintf(nomeFifo, "%s%c", path, fifoContent[1]);
    int fdScrittura = open(nomeFifo, O_WRONLY);

    char msg[110];
    char msgMas[100];
    strcpy(msgMas, parsing(fifoContent));
    sprintf(msg, "-%s", msgMas);
    if (write(fdScrittura, msg, sizeof(msg)) == -1)
        printf("Errore nell'invio\n");
    printf("[%d] Inviato: %s a [%c]\n", id, parsing(fifoContent), fifoContent[1]);
}

void stampa(char *fifoContent, int id)
{
    printf("[%d] Ricevuto: %s\n", id, parsing(fifoContent));
}

int main(int argc, char **argv)
{
    int idSlave = atoi(argv[0]);
    int maxSlave = atoi(argv[1]);

    while (1)
    {
        char path[10];
        char nomeFifo[100];
        strcpy(path, PATH);
        sprintf(nomeFifo, "%s%d", path, idSlave);

        int fdLettura = open(nomeFifo, O_RDONLY);
        char stringa[110];
        if (read(fdLettura, stringa, 110) == -1)
            printf("Errore nella lettura\n");

        if (stringa[0] == 'M')
        {
            inviaComando(stringa, idSlave);
        }
        else
        {
            stampa(stringa, idSlave);
        }
    }
}