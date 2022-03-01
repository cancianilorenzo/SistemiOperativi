#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/types.h> 
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>


#define CHILDREN_MAX 10

#define READ 0 
#define WRITE 1

extern int errno;

//-----------------------
int input[CHILDREN_MAX][2];
char pido[4];
int fd,master,manager;
int level;
int cont=0;
int br[CHILDREN_MAX];
int x=0;
//-----------------------

// prende il pid inviato dal processo e lo salva 
int pid_sender;
void counthandler(int signo, siginfo_t *info, void *csx){
    pid_sender = info->si_pid;
    x++;
    char appo[50];
    sprintf(appo,"%d",pid_sender);
    write(input[x][WRITE],&appo,strlen(appo));
}


void killChildren(){
    if(cont!=level){
        kill(br[cont],SIGKILL);
        cont++;
    }else{
        exit(0);
    }
}

void catcher( int sig ) {
    printf( "inside catcher() function\n" );
}

void killAllChildren(){
    for(int i =0;i<level;i++){
        kill(br[i],SIGTERM);
    }
    exit(0);
}

int main(int argc,char ** argv){
    if(argc<3){
        fprintf(stderr,"usage error\n");
        exit(3);
    }
    
    //controllo quanti figli devo creare
    level = atoi(argv[2]);
    if(level > CHILDREN_MAX || level <0){
        fprintf(stderr,"too much children\n");
        exit(4);
    }

    //creo il file se esiste errore 5
    if ((fd = open(argv[1], O_RDWR | O_CREAT | O_EXCL, 0777)) == -1){
        perror("Error: ");
        fprintf(stderr,"file opening failed\n");
        exit(5);
    }

    //sto scrivendo il pid sul file ARGV[1]
    sprintf(pido , "%d\n" , getpid());
    write(fd,pido,strlen(pido));
    
    master = getpid();
    //genero figlio
    manager = fork();
    if(master != getpid()){
        //manager
        struct sigaction sa;    
        sa.sa_sigaction = counthandler;  
        sa.sa_flags = SA_SIGINFO;
        sigemptyset(&sa.sa_mask);
        sigaction(SIGINT,&sa,0);

        struct sigaction sact;
        sigset_t new_set, old_set;

        sigemptyset( &sact.sa_mask );
        sact.sa_flags = 0;
        sact.sa_handler = catcher;
        sigaction( SIGALRM, &sact, NULL );

        sigemptyset( &new_set );
        sigaddset( &new_set, SIGALRM );
        sigprocmask( SIG_BLOCK, &new_set, &old_set);

        signal(SIGUSR1,killChildren);
        signal(SIGTERM,killAllChildren);
        
        sprintf(pido , "%d\n" , getpid());
        write(fd,pido,strlen(pido));

        for(int i =0;i<level;i++){
            pipe(input[i]);
            br[i] = fork();
            if(!br[i]){
                char buffer[50];
                sigemptyset( &new_set );
                sigaddset( &new_set, SIGCHLD );
                sigaddset( &new_set, SIGCONT );
                sigprocmask( SIG_BLOCK, &new_set, &old_set);

               
                sprintf(pido , "%d\n" , getpid());
                write(fd,pido,strlen(pido));
                close(input[i][WRITE]);
                int r =read(input[i][READ],&buffer,sizeof(buffer));
                buffer[r]='\0';
                int valore = atoi(buffer);
                kill(valore,SIGUSR2);
                break;
            }else{
                close(input[i][READ]);
            }
        }
        while(1);
    }

    close(fd);
    return 0;
}