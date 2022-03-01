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

#define READ 0
#define WRITE 1

int fd[2];

void *funzioneThread(void *param)
{
    char buffer[100];
    read(fd[READ], buffer, sizeof(buffer));
    printf("Letto da thread --> %s\n", buffer);
    return (void *)3; //valore arbitrario
}

int main(int argc, char **argv)
{

    pthread_t id_thread;
    pipe(fd);
    int numero = 0;
    pthread_create(&id_thread, NULL, funzioneThread, (void *)&numero);
    char buffer[100];
    strcpy(buffer, "ciao mamma");
    rite(fd[WRITE], buffer, sizeof(buffer));
    pthread_join(id_thread, NULL); 
    /*
    Se mettessi qui la write resterebbe bloccato
    Il thread aspetta qualcosa nella pipe
    Il main aspetta il thread
    */

    return 0;
}