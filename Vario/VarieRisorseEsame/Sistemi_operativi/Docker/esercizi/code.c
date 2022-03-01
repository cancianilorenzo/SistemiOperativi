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

struct messaggio{
    long mtype; //tipo messaggio, identifica la classe
    char mtext[1]; //campo testuale
} messaggio_padre;

struct msqid_ds mod; //struct dettagli

int main(int argc, char **argv){
    key_t coda_key = ftok("nome.file", 2);
    int coda_id = msgget(coda_key, 0777 | IPC_CREAT);
    msgctl(coda_id, IPC_RMID, NULL); //cancello e svuoto
    coda_id = msgget(coda_key, 0777 | IPC_CREAT);

    int processo=fork();
    if(processo==0){
        printf("[FIGLIO] = %d\n", getpid());
        struct messaggio{
            long mtype; //tipo messaggio, identifica la classe
            char mtext[10]; //campo testuale
        } messaggio;
        strcpy(messaggio.mtext,"Ciao"); //set testo
        messaggio.mtype=10; //set argomento
        msgctl(coda_id, IPC_STAT, &mod); //recupero valori coda
        mod.msg_qbytes=19; //cambio max bytes coda
        msgctl(coda_id, IPC_SET, &mod); //attuo modifiche coda
        int esito = msgsnd(coda_id, &messaggio, sizeof(messaggio.mtext), IPC_NOWAIT); //invio
        printf("Esito1: %d\n\n", esito);

    }else{
        sleep(1);
        //ricezione
        int esito = msgrcv(coda_id, &messaggio_padre, sizeof(messaggio_padre.mtext), 0, MSG_NOERROR);
        printf("(%d) %s\n", esito, messaggio_padre.mtext);
    }
}