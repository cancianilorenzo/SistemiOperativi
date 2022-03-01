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

#define LETTURA 0
#define SCRITTURA 1

int main(int argc, char **argv){

   int fd; char * fifoName = "ciao.txt";
   char str1[80],* str2 = "ciao"; 
   mkfifo(fifoName,S_IRUSR|S_IWUSR); //Create pipe if doesn’t exist 

   int processo = fork();
   if(processo==0){
      int fd = open(fifoName, O_RDONLY); // Open FIFO for write only 
      read(fd, str1, strlen(str1));
      printf("%s\n", str1);
      close(fd);
      sleep(2);
      fd = open(fifoName, O_RDONLY); // Open FIFO for write only 
      read(fd, str1, strlen(str1));
      printf("%s\n", str1);
      close(fd);
   }else{
      fd = open(fifoName, O_WRONLY); // Open FIFO for write only 
      write(fd, str2, strlen(str2));
      close(fd);
      sleep(1);
      fd = open(fifoName, O_WRONLY); // Open FIFO for write only 
      write(fd, "a", strlen("a"));
      close(fd);
      
   }


   printf("sono uscito\n");

}

