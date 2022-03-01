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

char *stringrev(char *str)
{
    int dimensione = strlen(str);
    char invertita[dimensione];
    int corrente = 0;
    for (int i = dimensione - 1; i > -1; i--)
    {
        invertita[corrente] = str[i];
        corrente++;
    }
    strcpy(str, invertita);
    str[dimensione] = '\0'; //Terminatore di stringa
    return str;
}

int stringpos(char *str, char chr)
{
    int dimensione = strlen(str);
    int posizione = 0;
    for (posizione = 0; posizione < dimensione; posizione++)
    {
        if (str[posizione] == chr)
        {
            return posizione+1;
        }
    }
}

int main(int argc, char **argv)
{
    int posizione = stringpos(argv[1], argv[2][0]);
    printf("Invertita --> %s\n", stringrev(argv[1]));
    printf("e il carattere %c si trova in posizione %d\n", argv[2][0], posizione);
    return 0;
}