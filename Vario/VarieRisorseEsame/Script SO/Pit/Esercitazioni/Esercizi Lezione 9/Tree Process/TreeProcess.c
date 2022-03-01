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

#define MAX_LIVELLI 5

int gid_livello[MAX_LIVELLI];

void main(int argc, char **argv)
{

    for(int i = 0; i < MAX_LIVELLI; i++){
        int figlio = fork();
        if(figlio == 0){
            //Sono nel padre
        }
        else{
            
        }
    }

    while (1)
    {
        sleep(1);
        char comando[2];
        printf("inserisci prossimo comando: ");
        fflush(stdout);
        scanf("%s", comando);
        //printf("ricevuto %s\n", comando);
        int livello;
        switch (comando[0])
        {
        case 'c':
            livello = comando[1]-'0';
            printf("Livello --> %d\n", livello);

            //prendi livello e crea figlio
            break;
        case 'k':
            livello = comando[1]-'0';
            printf("Livello --> %d\n", livello);
            //prendi livello e termina figli (tutti?)
            break;
        case 'p':
            //Stampa livelli
            break;
        case 'q':
            //termina tutti i figli e il processo
            break;
        default:
            fprintf(stderr, "Comando non valido!!\n");
        }
    }
}