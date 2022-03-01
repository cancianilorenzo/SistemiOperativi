//      ./eseguibile.o -c -l <path>
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
#include <stdbool.h>

#define MAX_ARG 10
#define MAX_ARG_OPT 2
#define MAX_LENG 30
#define IGNORE_BLANK_INPUT 1

extern int errno;
int nc = 0;
int nl = 0;
int msg;
int narg;
int f;
int st;
FILE* content;
char options[]= {'x', 'x'}; 
char filename[MAX_LENG]="";

int quit();
int direct();
int piping();
void stampa();


int main(int argc, char *argv[])
{
    int opt;
    int i=0;

// ----------------------------------- CHECK ARGUMENTS ----------------------------------- 
    narg= argc-1;
    if(narg>MAX_ARG){ quit(3); }
    bool primo=false; bool secondo=false;

// ----------------------------------- PARSING ARGUMENTS ----------------------------------- 
	while((opt = getopt(argc, argv, ":cl")) != -1 && i < MAX_ARG_OPT ){
		switch(opt)
		{
			case 'c':
                if(!primo){ options[0]='c'; primo=true; break; }
                if(primo && !secondo){ options[1]='c'; secondo=true; break; }
			case 'l':
                if(!primo) { options[0]='l'; primo=true; break; }
                if(primo && !secondo){ options[1]='l'; secondo=true; break; }
                
		}
        i++;
	}

    for(; optind < argc; optind++){	
        strcpy(filename, argv[optind]);
        break;
	}
// ---------------------------------------- FORK ----------------------------------------- 
    f=fork();
    if(f<0) quit(6);
    if(f==0){
// ----------------------------------- PIPING or DIRECT ----------------------------------- 
        int verifica= isatty(fileno(stdin));
        if(verifica){
            direct();
        }else{
            piping(); 
        }
// ---------------------------------------- STAMPA ----------------------------------------- 
        stampa();
    }else{
        while(wait(&st)>0);
        return WEXITSTATUS(st);
    } 
}

int quit(int err) {
    char msg[36]="?unknown";
    switch (err) {
            case  0 : strcpy(msg, "");                           break;
            case  2 : strcpy(msg, "?Wrong number of arguments"); break;
            case  3 : strcpy(msg, "?Wrong argument 'MAX_ARG'");  break;
            case  4 : strcpy(msg, "?'path' too long");           break;
            case  5 : strcpy(msg, "?Error in path");             break;
            case  6 : strcpy(msg, "?Fork error");                break;
            case  7 : strcpy(msg, "?Error creating txt");        break;
            case  8 : strcpy(msg, "?Error file");                break;
            case  9 : strcpy(msg, "?Error in queue");            break;
            case 10 : strcpy(msg, "?Global quit");               break;
            default: strcpy(msg, "?Generic error");              break;
        }
        fprintf(stderr, "?Error #%d: %s\n", err, msg);
        exit(err);   
}

int direct(){
    char c;
    if(strcmp(filename, "")==0) { if(IGNORE_BLANK_INPUT) return 0; else quit (3); }
    content = fopen(filename, "r");
    if(content==NULL) quit(8);
    while(content != NULL){
        c= fgetc(content);
        if(feof(content)) break;
        nc++;
        if(c=='\n') nl++;
    }
    if(content!=NULL) fclose(content);
    return 0;
}

int piping(){
    char c;
    while(read(STDIN_FILENO, &c, 1)){
        nc++;
        if(c=='\n') nl++;
    }
    return 0;
}

void stampa(){
    for(int i=0; i<MAX_ARG_OPT; i++){
        if(options[i]=='c') printf("%d ", nc);
        if(options[i]=='l') printf("%d ", nl);
        if(options[i]=='x') {} //printf(" ");
    }
    printf("\n");
    
}