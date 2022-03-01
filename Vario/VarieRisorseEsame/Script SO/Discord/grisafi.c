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
    printf("Sono nel thread\n");
    while(1);
}

void main()
{
    pthread_t t_id;
    pthread_create(&t_id, NULL, my_fun, NULL);

    //pthread_join(t_id, NULL);
    while(1);
}
