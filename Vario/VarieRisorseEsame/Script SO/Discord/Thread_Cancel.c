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
    pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL); //Change type
    pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL); //Change mode
    //pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL); //Change mode
    //pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL); //Change type
    sleep(2); //Cancellation point
    printf("Thread %ld started\n", (pthread_t *)param);
    
    int valore = 3;
    pthread_exit((void *)valore);
}
void main()
{
    void *ritorno;
    pthread_t t_id;
    pthread_create(&t_id, NULL, my_fun, (void *)&t_id);
    pthread_cancel(t_id);

    pthread_join(t_id, &ritorno);

    if (ritorno == PTHREAD_CANCELED)
    {
        printf("Thread %ld: %d\n", t_id, (int *)ritorno);
    }
    else{
        printf("Thread %ld: %d\n", t_id, (int *)ritorno);
    }
}