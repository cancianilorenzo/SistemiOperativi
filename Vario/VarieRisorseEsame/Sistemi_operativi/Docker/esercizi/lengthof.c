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

int lunghezzaStringa(char* stringa){
    int lunghezza = 0;
    char c = stringa[lunghezza];
    while(c != '\0'){
        lunghezza++;
        c = stringa[lunghezza];
    }
    return lunghezza;
}


int main(int argc, char** argv){
    printf("Inserita --> %s\n", argv[1]);
    printf("La lunghezza della stringa è %d \n", lunghezzaStringa(argv[1]));
    return 0;
}