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
    system("ls");
    sleep(1);
    mkdir("ciao", 0777);
    sleep(1);
    system("ls");
    char path[100];
    strcpy(path, "ciao/percorsoCartella");
    mkdir(path, 0777);
    sleep(1);
    system("ls");
}