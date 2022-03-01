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

#define MAX_INT 10
#define MIN_INT 1
#define ARGOMENTI 2
#define MAX_LENG 100

char target[MAX_LENG]="";
int proc_foglie[MAX_INT];
int num;
int narg;
int fd_file;
int f;
int status;
int foglie;
int foglie_attive;
int temp=0;
int p[2], pid, nbytes;        
int esito;
int temp;


struct sigaction sa;
struct sigaction sa_foglie;

int quit();


void handler(int signo, siginfo_t* info, void* empty){                                                       //handler manager
    if(signo==SIGUSR1){
        if(foglie_attive>0){

            char pid_richiesta[MAX_LENG];   
            snprintf(pid_richiesta, MAX_LENG, "%d", info->si_pid); 
            if(write(p[1], pid_richiesta, strlen(pid_richiesta))==-1) {quit(8);}                            //write on pipe pid request

            kill(proc_foglie[foglie_attive-1], SIGUSR1);                                                    //send signal to child                  
            sleep(1);
            foglie_attive--;
            printf("[SIGUSR1] foglie attive: %d\n", foglie_attive);                                         //debug output
            if(foglie_attive==0) quit(0);                                                                   //quit if no child alive
        }else{
            quit(0);
        }
    }
    if(signo==SIGTERM){
        printf("[SIGTERM] termine foglie e manager\n");                                                     //debug output
        /*for(int i=0; i<foglie_attive-1; i++){
            kill(proc_foglie[i], SIGKILL);
        }*/
        kill(-proc_foglie[0], SIGKILL);
        sleep(2);
        quit(0);
    }

} 

void handler_foglia(int signo, siginfo_t* info, void* empty){                                               //handler manager
    if(signo==SIGUSR1){
        char buffer[MAX_LENG];                                  
        int c = read(p[0], &buffer, MAX_LENG);
        kill(atoi(buffer), SIGUSR2);                                                                        //send signal to request
        sleep(1);
        quit(0);                                                                                            //quit signal
    }

} 

int child() {
    
    sa_foglie.sa_sigaction = handler_foglia;                                                                //set sigaction child
    sigemptyset(&sa_foglie.sa_mask);
    sigaddset(&sa_foglie.sa_mask, SIGCHLD);                                                                
    sigaddset(&sa_foglie.sa_mask, SIGCONT);      
    sa_foglie.sa_flags |= SA_SIGINFO;                                                                       
    sigaction(SIGUSR1, &sa_foglie, NULL);

    //setpgid(33, getpid());
    char pid_foglia[100];
    snprintf(pid_foglia, 100, "%d\n", getpid()); 
    if(write(fd_file, pid_foglia, strlen(pid_foglia))==-1) { quit(8); }                                     //write pid on file
	
    while(1);                                                                                               //keep process active
	return 0;
}
int main(int argc, char *argv[]){

// ----------------------------------- CHECK ARGUMENTS ----------------------------------- 
    narg= argc-1;
    if(narg!=ARGOMENTI) quit(3);
    strcpy(target, argv[1]);
    foglie=atoi(argv[2]);

    if(foglie>=MIN_INT && foglie<=MAX_INT){
        num=foglie;
    }else{ 
        quit(4); 
    }

    fd_file=open(target, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);                                              //file open

    if(fd_file == -1) quit(5);
    char pid_padre[100];
    snprintf(pid_padre, 100, "%d\n", getpid());
    if(write(fd_file, pid_padre, strlen(pid_padre))==-1) {quit(8);}                                         //write pid father

// ---------------------------------------- FORK ----------------------------------------- 
    f=fork();
    if(f==0){                                                                                               //manager

        sa.sa_sigaction = handler;  
        sigemptyset(&sa.sa_mask);     
        sa.sa_flags |= SA_SIGINFO;
        sigaddset(&sa.sa_mask, SIGALRM);                                                                    //sigaction manager
        sigaction(SIGUSR1, &sa, NULL);
        sigaction(SIGTERM, &sa, NULL);
                                                                 
        char pid_manager[100];
        snprintf(pid_manager, 100, "%d\n", getpid()); 
        if(write(fd_file, pid_manager, strlen(pid_manager))==-1) {quit(8);}                                 //write pid manager on file

        proc_foglie[foglie];
        foglie_attive=foglie; 

        char inbuf[MAX_LENG];
        esito = pipe(p);                                                                                    //creation pipe
        if(esito!=0) quit(10);                                                                                   

        for(int i=0; i<foglie; i++){                                                                        //fork
            f=fork();   
            if(f==0){ 
                child();
            }else{
                proc_foglie[i]=f;
                setpgid(proc_foglie[i],proc_foglie[0]);
            }
        }
        while(1);
    }else{
        quit(0);                                                                                           //father exit
    }
}

int quit(int err) {
    char msg[100]="?unknown";
    switch (err) {
            case  0 : strcpy(msg, "");                                                  break;
            case  2 : strcpy(msg, "?Wrong number");                                     break;
            case  3 : strcpy(msg, "?Errore numero argomenti");                          break;
            case  4 : strcpy(msg, "Errore numero non valido [1-10]");                   break;
            case  5 : strcpy(msg, "?Errore apertura file o path inesistente");          break;
            case  6 : strcpy(msg, "?Fork error");                                       break;
            case  7 : strcpy(msg, "?Error creating txt");                               break;
            case  8 : strcpy(msg, "?Errore scrittura nel file ");                       break;
            case  9 : strcpy(msg, "?Error in queue");                                   break;
            case 10 : strcpy(msg, "?Errore creazione pipe");                            break;
            default: strcpy(msg, "?Generic error");                                     break;
        }
        if(err>0) fprintf(stderr, "Code: %d %s\n", err, msg);
        exit(err);   
}