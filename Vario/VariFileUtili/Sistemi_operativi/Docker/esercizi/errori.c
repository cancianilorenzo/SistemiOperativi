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

extern int errno;

int main(int argc, char **argv){
        FILE * pf = fopen ("nonesiste.boh", "r");
        if(pf == -1) quit(3);
        if (pf == NULL) {
            //intero 
            fprintf(stderr, "errno: %d\n", errno); 
            //stringa relativa all'intero
            fprintf(stderr, "strerror: %s\n", strerror(errno));  
            //stampa direttamente la stringa            
            perror("Ciao");   
        }else {
            fclose (pf);
        }
}

void quit(int err) {
    char msg[100];
	switch (err) {
		case  0 : strcpy(msg, "");                           break;
		case  2 : strcpy(msg, "?Wrong number of arguments"); break;
		case  3 : strcpy(msg, "?Wrong argument 'n'");        break;
		case  4 : strcpy(msg, "?'path' too long");           break;
		case  5 : strcpy(msg, "?Error in path");             break;
		case  6 : strcpy(msg, "?Fork error");                break;
		case  7 : strcpy(msg, "?Error creating txt");        break;
		case  8 : strcpy(msg, "?Error writing log");         break;
		case  9 : strcpy(msg, "?Error in queue");            break;
		case 10 : strcpy(msg, "?Global quit");               break;
		default: strcpy(msg, "?Generic error");              break;
	};
	if (err>0) { //errori su stderr
         fprintf(stderr, "%s", msg); 
    } else { //successo su stdout
        printf("%s\n", msg); 
    };
	fflush(stderr); fflush(stdout);
	exit(err);
};
