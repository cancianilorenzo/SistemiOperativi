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
#include <time.h>

#define MAX_FIGLI 5
#define LETTURA 0
#define SCRITTURA 1

int fd_1[MAX_FIGLI][2]; //figlio legge
int fd_2[MAX_FIGLI][2]; //padre legge

int num_figli;

int child(int i){
    while(1){
        char lettera[1];
        int r = read(fd_1[i][LETTURA], &lettera, sizeof(lettera));
        if(lettera[0] == 'i') {
            char pid[10];
            sprintf(pid, "%d", getpid());
            write(fd_2[i][SCRITTURA], pid, sizeof(pid));
        }
        if(lettera[0] == 'r'){
            srand(time(NULL)); 
            int ran = rand();
            char random[100];
            sprintf(random, "%d", ran);
            write(fd_2[i][SCRITTURA], random, sizeof(random));
        }
    }
    return 0;
}

int quit(int err) {
    char msg[100]="?unknown";
    switch (err) {
            case  0 : strcpy(msg, "");                                                  break;
            case  1 : strcpy(msg, "?Numero argomenti non valido");                      break;
            case  2 : strcpy(msg, "?Numero figli non valido");                          break;
            case  3 : strcpy(msg, "?Comando non valido");                               break;
        }
        if(err>0) fprintf(stderr, "Code: %d %s\n", err, msg);
        exit(err);   
}


int main(int argc, char ** argv){
    if(argc < 2 || argc > 2){ quit(1);}

    num_figli = atoi(argv[1]);
    if(num_figli <1 || num_figli> MAX_FIGLI){ quit(2); }

    printf("-- Childer: %d\n", num_figli);
    int processi[num_figli];
    int processo;
    for(int i=0; i < num_figli; i++){
        pipe(fd_1[i]);
        pipe(fd_2[i]);
        processo=fork();
        if(processo == 0){
            close(fd_2[i][LETTURA]);
            close(fd_1[i][SCRITTURA]);
            child(i);
        }else{
            close(fd_1[i][LETTURA]);
            close(fd_2[i][SCRITTURA]);
            processi[i]=processo;
        }
    }

    while(1){
        sleep(2);
        char command[3];
        printf("Next command: "); fflush(stdout);
        scanf("%s", command);
        char lettera;
        int numero;

        lettera=command[0];
        numero=command[1]-'0';

        if(lettera =='q'){
            for(int i=0; i<num_figli; i++){
                kill(processi[i], SIGKILL); //Soluzione veloce, in alternativa si può anche mandare q e far terminare il figlio.
            }
            quit(0);
        }else if(lettera =='i' || lettera =='r' ){
            char stringa[1]; stringa[0]=lettera;
            int value = write(fd_1[numero-1][SCRITTURA], stringa, sizeof(stringa));
        }
        
        char buffer[100];
        read(fd_2[numero-1][LETTURA], buffer, sizeof(buffer));
        printf("Ricevuto da %d : %s\n", processi[numero-1], buffer);

    }
}