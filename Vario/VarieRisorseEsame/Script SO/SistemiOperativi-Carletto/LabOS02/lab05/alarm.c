#include <signal.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> //alarm.c 
short cnt = 0;


void myHandler(int sigNum){
    printf("ALARM!\n"); 
    exit(0);
    }

int main(){
    signal(SIGALRM,myHandler);
    alarm(5); //Set alarm in 5 seconds
    //Set new alarm (cancelling previous one)
    //printf("Seconds remaining to previous alarm %d\n",alarm(2)); 
    while(1);

    return 0;
}