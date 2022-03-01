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

void *my_fun(void *param)
{
    printf("Funzione thread 1\n");
    int valore = 2;
    pthread_exit((void *)valore);
}

void *my_fun1(void *param)
{
    pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);     //Change mode
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL); //Change type
    printf("Thread %ld started\n", (pthread_t *)param);
    sleep(2);
    int valore = 3;
    pthread_exit((void *)valore);
}
void main()
{
    void *ritorno;
    pthread_t t_id1, t_id2;
    pthread_create(&t_id1, NULL, my_fun1, (void *)&t_id1);
    pthread_create(&t_id2, NULL, my_fun, (void *)&t_id2);
    pthread_cancel(t_id2);
    pthread_join(t_id2, &ritorno);
    printf("Thread %ld: %ls\n", t_id2, (int *)ritorno);
    //pthread_join(t_id1, NULL);
}