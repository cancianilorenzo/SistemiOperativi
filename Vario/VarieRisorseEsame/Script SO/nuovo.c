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

int main(){
    int fd = open("file.txt", O_CREAT | O_RDWR, S_IRUSR|S_IWUSR);

    write(fd, "Ciao mamma", strlen("Ciao mamma"));
    //lseek(fd, 0, SEEK_END);
    //printf("%ld\n", lseek(fd, 0, SEEK_CUR));
    /*int processo = fork();

    if (processo == 0){
         int fd_2 = open("file.txt", O_CREAT | O_RDWR, S_IRUSR|S_IWUSR);
          lseek(fd_2, 0, SEEK_SET); //Porto all'inizio
          write(fd_2, "casa\n", strlen("casa\n"));
          printf("Figlio %ld\n", lseek(fd_2, 0, SEEK_CUR));
    }
    else{
        //sleep(1);*/
        //printf("%ld\n", lseek(fd, 0, SEEK_CUR));
        //write(fd, "Ciao mamma", strlen("Ciao mamma"));
        char buffer[100];
        int letti = read(fd, buffer, sizeof(buffer));
        char stringa[letti];
        strcpy(stringa, buffer);
        printf("Letto da padre --> %s (%d)\n", buffer, letti);
    //}
   
    return 0;
}