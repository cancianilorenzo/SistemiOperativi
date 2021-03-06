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

#define RED "\033[0;31m"
#define DF "\033[0m" 

#define MAX_CHILDERN 5
#define LETTURA 0
#define SCRITTURA 1

char memoria[50];
int processi[MAX_CHILDERN];
int file;
int padre;

int children=0;
int f;

pthread_t id_thread;
struct sigaction sa;
int fd[MAX_CHILDERN][2]; 
pthread_attr_t attr;


void *funzione(void *signo)
{
    //write(file,"\033[0;31m[THREAD] I'm a new thread generated by signal %d\n",*(int *)param);
    write(file,"[THREAD] Sending message to children\n", sizeof("[THREAD] Sending message to children\n"));
    for(int i=0; i<children; i++){
        char segnale[100];
        sprintf(segnale, "%d\n", *(int *)signo);
        write(file, segnale, sizeof(segnale));
        char mex[100];
        sprintf(mex, "%d  %d %s", processi[i], i, memoria);
        write(fd[i][SCRITTURA], mex, strlen(mex));
        close(fd[i][SCRITTURA]); 
    }
    children=0;
    write(file,"[THREAD] children reset\n", sizeof("[THREAD] children reset\n")); 
    write(file,"[THREAD] Terminating thread\n", sizeof("[THREAD] Terminating thread\n"));
}

static void handler(int signo){
    int arg;
    switch (signo) {
            case SIGUSR1:
                arg=SIGUSR1;
                pthread_create(&id_thread, NULL, funzione, (void *)&arg); 
            break;
            case SIGUSR2:
                arg=SIGUSR2;
                pthread_create(&id_thread, NULL, funzione, (void *)&arg); 
            break;
    }
}


void nascita(){
    if(children<MAX_CHILDERN){
        pipe(fd[children]);

        int f=fork();
        if(f==0){
            close(fd[children][SCRITTURA]);
            printf("\033[0;33m[CHD] I'm a new child with id %d, and I'm waiting for msg from my father\033[0;37m\n",getpid());
            while(1){
                char buffer[100];
                int c = read(fd[children][LETTURA], &buffer, sizeof(buffer));
                printf("\033[0;33m[CHD] I received the following message: %s \033[0;37m\n",buffer);
                close(fd[children][LETTURA]);
                exit(1);
            }
        }else{
            close(fd[children][LETTURA]);
            processi[children]=f;
            children++;
        }
    }else{
        fprintf(stderr,"\033[0;32m[MAIN] Too many children. Send msg to free them \033[0;37m\n");
    }
}

int main(int argc, char **argv){
  
    sa.sa_handler = handler;
    sigemptyset(&sa.sa_mask);   
    sigaction(SIGUSR1, &sa, NULL); 
    sigaction(SIGUSR2, &sa, NULL);
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);
    printf("\033[0;32m[PADRE] %d\033[0;37m\n", getpid());
    padre=getpid();

    file = open("log.txt", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);    
    char comando[100];
    int intCmd, r;

    while(1){
        sleep(2);
        printf("%s--- Figli attivi: %d%s\n", RED, children, DF);
        r = read(STDIN_FILENO, comando, 100-1); //Read from stdinput
        comando[r-1] = 0; //Terminate string
        intCmd = atoi(comando);
        if(intCmd>0){            //number
            nascita();
        }else{                          //string
            strcpy(memoria, comando);
            fprintf(stderr,"\033[0;32m[MAIN] Msg saved \033[0;37m\n");
        }
    }    
}
