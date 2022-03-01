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

static void handler(int signo){
    switch (signo) {
            case SIGTERM:
                printf("RICEVUTO SIGUSR1 [PID]: %d\n", getpid());
                sleep(1);
                exit(0);
            break;
    }
}


int pidGruppo;
int main(int argc, char **argv){
    
    printf("-- PADRE %d\n", getpid());
    int valore = fork();
    if (valore == 0)
    {   
        struct sigaction sa; //struct sigaction
        sa.sa_handler = handler; //non utilizzo informazioni
        sigemptyset(&sa.sa_mask);   //svuotare maschera
        sigaction(SIGTERM, &sa, NULL); //segnale gestito in handler

        printf("-- FIGLIO1 %d\n", getpid());
        printf("-- 1 %d\n", getpgid(getpid()));
        //sono nel figlio
        setpgid(getpid(), 0);
        printf("-- 2 %d\n", getpgid(getpid()));
        while(1);
    }else{
        pidGruppo=valore;
    }


    int fork1 = fork();
    if (fork1 == 0){
        struct sigaction sa; //struct sigaction
        sa.sa_handler = handler; //non utilizzo informazioni
        sigemptyset(&sa.sa_mask);   //svuotare maschera
        sigaction(SIGTERM, &sa, NULL); //segnale gestito in handler
        //sono nel figlio
        printf("-- FIGLIO2 %d\n", getpid());
        printf("-- 3 %d\n", getpgid(getpid()));
        printf("Valore gruppo: %d\n", pidGruppo);
        setpgid(getpid(), valore);
        printf("-- 4 %d\n", getpgid(getpid()));
        while(1);
    }
    sleep(3);
    kill(-valore, SIGTERM);
    return 0;
}