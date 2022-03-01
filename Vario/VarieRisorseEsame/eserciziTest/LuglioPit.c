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

#define MAX_ARG 2                                                                       // max rgomenti
#define MAX_LENG 100                                                                    // max lunghezza stringhe


char path[MAX_LENG];                                                                    // <path>/info
char path_file[MAX_LENG];                                                               // <path>/info/key.txt
struct sigaction sa;                                                                    // sigaction children
key_t coda_key;                                                                         // chiave coda
int coda_id;                                                                            // id coda
int file;                                                                               // file descriptor

struct messaggio{
            long mtype;                                                                 
            char mtext[100];                                                            
} messaggio, ricevuto;

// ----------------------------------- ERROR ----------------------------------- 
int quit(int err) {
    char msg[100]="?unknown";
    switch (err) {
            case  0 : strcpy(msg, "");                                                  break;
            case  1 : strcpy(msg, "?Argomento 2 non valido");                           break;
            case  2 : strcpy(msg, "?Argomento 1 non valido");                           break;
            case  3 : strcpy(msg, "?Numero argomenti non valido");                      break;
            case  4 : strcpy(msg, "Errore creazione cartella");                         break;
            case  5 : strcpy(msg, "?Errore creazione file");                            break;
            case  6 : strcpy(msg, "?Errore scrittura su file");                         break;
            case  7 : strcpy(msg, "?Errore scrittura segnale");                         break;
            case  8 : strcpy(msg, "?Errore scrittura nel file ");                       break;
            case  9 : strcpy(msg, "?Error in queue");                                   break;
            case 10 : strcpy(msg, "?Errore creazione pipe");                            break;
            default: strcpy(msg, "?Generic error");                                     break;
        }
        if(err>0) fprintf(stderr, "Code: %d %s\n", err, msg);
        exit(err);   
}

// ----------------------------------- HANDLER CHILD ----------------------------------- 
void handler(int signo){
    if(signo == SIGUSR1){
       if(write(file, "SIGUSR1\n", sizeof("SIGUSR1\n")) == -1) quit(7);  
    }
    if(signo == SIGUSR2){ 

        coda_id = msgget(coda_key, 0777);
        char pid[100];
        sprintf(pid, "%d", getpid());
        strcpy(messaggio.mtext, pid);
        messaggio.mtype=1;
        int esito = msgsnd(coda_id, &messaggio.mtext, sizeof(messaggio.mtext), 0);
        printf("-- Esito Coda Figlio: %d\n", esito);
    }
}

// ----------------------------------- CHILD ----------------------------------- 
int child(){ 

    sa.sa_handler = handler;  
    sigemptyset(&sa.sa_mask);     
    sigaction(SIGUSR1, &sa, NULL);
    sigaction(SIGUSR2, &sa, NULL);
    
    char path_child[MAX_LENG];
    strcpy(path_child, path);
   
    char pid_figlio[20];
    sprintf(pid_figlio, "/%d.txt", getpid());
    strcat(path_child, pid_figlio);
    file = open(path_child, O_WRONLY | O_CREAT, 0755); if(file == -1) quit(5);

    while(1);
    return 0;
}

int main(int argc, char *argv[]){

    system("rm -r tmp/info");
    system("clear");
    system("mkdir tmp");
// ----------------------------------- CHECK ARGUMENTS ----------------------------------- 
    int num = atoi(argv[2]);
    int numarg = argc;
    if(numarg!=3) quit(3);
    strcpy(path, argv[1]);
    //TODO:controllo path
    if(num<1 || num>10) quit(1);

// ----------------------------------- FOLDER /info ----------------------------------- 
    //TODO: contollo che non ci sia / al termine della stringa
    strcat(path, "/info");
    if(mkdir(path, 0775) == -1){ quit(4);}
    printf("-- Path0: %s\n", path);

// ----------------------------------- FILE /key.txt ----------------------------------- 
    strcpy(path_file, path);
    printf("-- Path1: %s\n", path_file);
    strcat(path_file, "/key.txt");
    printf("-- Path2: %s\n", path_file);
    file = open(path_file, O_WRONLY | O_CREAT, 0755); if(file == -1) quit(5);
    
    char pid_principale[100];
    snprintf(pid_principale, 100, "%d\n", getpid()); 
    if(write(file, pid_principale, strlen(pid_principale))==-1) {quit(6);}   
    

    //creo coda
    coda_key = ftok(path_file, 32);
    coda_id = msgget(coda_key, 0777 | IPC_CREAT | IPC_EXCL);
    if(coda_id == -1){
        msgctl(coda_id, IPC_RMID, NULL);
    }
    coda_id = msgget(coda_key, 0777 | IPC_CREAT);
    
    //strcpy(pid_principale,"");
    sprintf(pid_principale, "%d", getpid());
    strcpy(messaggio.mtext, pid_principale);
    messaggio.mtype=1;
    int esito = msgsnd(coda_id, &messaggio, sizeof(messaggio.mtext), 0);
    printf("-- Esito Coda: %d\n", esito);

    esito = msgrcv(coda_id, &ricevuto, sizeof(ricevuto.mtext), 1, 0); //leggo dalla QUEUE
    printf("Ricevuto padre (%d) %s\n", esito, ricevuto.mtext);


    /*
    key_t coda_key = ftok(file_key, 32);
    int id_coda = msgget(coda_key, 0777 | IPC_CREAT);
    msgctl(id_coda, IPC_RMID, NULL);
    id_coda = msgget(coda_key, 0777 | IPC_CREAT);
    messaggio.mtype = 1;
    char coda_msg[PIDLEN];
    sprintf(coda_msg, "%d", getpid());
    strcpy(messaggio.mtext, coda_msg);
    int esito = msgsnd(id_coda, &messaggio, sizeof(messaggio.mtext), 0);
    esito = msgrcv(id_coda, &ricevuto, sizeof(ricevuto.mtext), 1, 0); //leggo dalla QUEUE
    printf("Ricevuto padre (%d) %s\n", esito, ricevuto.mtext);*/

// ----------------------------------- FORK ----------------------------------- 
    printf("--[PADRE] %d\n", getpid());

    int processi[num];
    int figlio;
    for(int i=0; i<num; i++){
        figlio = fork();
        if(figlio==0){
            child();
        }else{
            processi[i]=figlio;
        }
    }

// ----------------------------------- PRINT CHILDREN ----------------------------------- 
    for(int i=0; i<num; i++){
        printf("(%d) %d\t", i, processi[i]);
    }
    printf("\n");
/*
    while(1){
        int esito = msgrcv(coda_id, &messaggio, sizeof(messaggio.mtext), 1, 0); //leggo dalla QUEUE
        printf("(%d) %s\n", esito, messaggio.mtext);
    }     
*/
    return 0;
}