/*
Creo primo figlio
aspetto il termine
creo il secondo che legge dal file temporaneo creato dal primo
rimuovo il file temporaneo(?)
*/
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>


int main(int argc, char** argv){

int fd = open("temporaneo.txt", O_CREAT|O_RDWR, S_IRUSR|S_IWUSR); //Va creato dagli altri processi


    int primoFiglio = fork();
    if(primoFiglio == 0){
        //sono nel figlio
        dup2(fd,1);
        execv(argv[1], argv);

        return 0;
    }
    while(wait(NULL)>0);
    int secondoFiglio = fork();
    if(secondoFiglio == 0){
        //sono nel figlio
        dup2(fd,0);
        execv(argv[2], argv);

        return 0;
    }
    while(wait(NULL)>0);
    return 0,

}