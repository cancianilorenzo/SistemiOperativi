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
    //pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL); //possiamo cancellarlo
    //pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL); //evita la cancellazione 
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,NULL); //Change type 
    
    printf("Thread %ld started\n",(pthread_t *)param); 
    //sleep(2);
    int valore = 0;
    pthread_exit((void *)valore);
}
void main(){
    void * ritorno;
    pthread_t t_id;
    pthread_create(&t_id, NULL, my_fun, (void *)&t_id);
    pthread_cancel(t_id);
    pthread_join(t_id, &ritorno);
    if(ritorno = PTHREAD_CANCELED){
        printf("Thread %ld: %d\n", t_id, (int *)ritorno);
    }

}