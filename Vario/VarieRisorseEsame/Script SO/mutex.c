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

pthread_mutex_t lock; 
pthread_t tid[2];
int counter = 0;
void *thr1(void *arg){
    pthread_mutex_lock(&lock);
    printf("Thread %ld has %d\n", (long)arg, counter);
    counter += 1;
    printf("Thread %ld has %d\n", (long)arg, counter);
    pthread_mutex_unlock(&lock);

}

void main(void){

    pthread_mutex_init(&lock, NULL);
    pthread_create(&(tid[0]), NULL, thr1,(void *)&tid[0]);
    pthread_create(&(tid[1]), NULL, thr1,(void *)&tid[1]);
    pthread_join(tid[0], NULL);
    pthread_join(tid[1], NULL);
    pthread_mutex_destroy(&lock);
}