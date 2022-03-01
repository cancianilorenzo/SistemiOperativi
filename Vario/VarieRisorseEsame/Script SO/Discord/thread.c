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

void *funzioneThread(void *param)
{
    printf("Valore --> %d\n", *(int *)param);
    sleep(5);
    return (void *)3; //valore arbitrario
}

int main(int argc, char** argv)
{
    int numero = 5;
    pthread_t id_thread;
    pthread_create(&id_thread, NULL, funzioneThread, (void *)&numero);
    pthread_join(id_thread, NULL);
    return 0;
}