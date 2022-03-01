#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <sys/wait.h>
#include <string.h>

#define MAX_PROC 20 //Number of process that can be created
#define RD 0
#define WR 1
#define MAX_ARG 3 //Number of chars for stdin commands

#define RED "\033[0;31m"
#define GREEN "\033[32m"
#define DF "\033[0m"



// Error printing function
void perr(char * str){
    fprintf(stderr,"%s%s%s",RED,str,DF);
}

//Termination function
void signalIntHandler(int signo){
    //Issue termination signal to all children
    kill(0,SIGTERM);

    //Wait children termination
    while(wait(NULL)>0);
    printf("Terminating");

    //Terminate
    exit(0);
}
int main(int argc, char ** argv){
    
    //Get process number from first parameter
    if(argc < 2){
        perr("Wrong number of parameters\n");
        return 1;
    }

    //Convert argument to integer
    int proc = atoi(argv[1]);
    if(proc > 0 && proc <= MAX_PROC){
        printf("Creating %d processes\n",proc);
    }else{
        perr("Wrong number of proc\n");
        return 2;
    }
    int current = proc;
    int isChild, r, randomFromChildren;
    char command[MAX_ARG+1]; //Buffer for input parameters
    char buffer[255]; //Buffer for communication

    //Children ids
    pid_t children[MAX_PROC];
    // Input pipes used by the children to talk with the parent
    int input[MAX_PROC][2];
    
    //Output pipes used by the parent to talk with the childrens
    int output[MAX_PROC][2];

    //Cycle over the number of procs. 
    for(;current>0; current--){
        //Create pipes
        pipe(input[current]);
        pipe(output[current]);
        isChild = fork();
        if(isChild != 0){ // Ancestor
            children[current] = isChild;
            close(input[current][WR]);
            close(output[current][RD]);
        }else{ //Child
            close(output[current][WR]);
            close(input[current][RD]);
            break; //Break from the for loop if child. Child will have unique 'current' value
        }
    }

    if(isChild != 0){ //Ancestor
        signal(SIGINT,signalIntHandler); //Override handler to kill all children
        while(1){
            printf("\nNext command: ");fflush(stdout);
            r = read(STDIN_FILENO, command, MAX_ARG+1);
            if(command[0] == 'q'){
                // Terminate all children and itself
                signalIntHandler(0);
            }
            if(command[0] != 'r' && command[0] != 'i'){
                perr("Wrong parameter. Allowed are 'r' and 'i'\n");
                continue;
            }
            //Get target process
            int process = atoi(command+1);
            if(process < 0 || process > proc){
                perr("Wrong target\n");
                continue;
            }
            process--;
            //Instruct child to produce output
            write(output[process][WR],command,1);

            //Get random number from child
            r = read(input[process][RD],&randomFromChildren,sizeof(randomFromChildren));
            printf("Child %d told me: '%d'\n",children[process],randomFromChildren);
        }
        
    }else{ //Child
        srand(current);   // Initialization, should only be called once.
        while(1){ 
            int random = rand(); // Returns a pseudo-random integer between 0 and RAND_MAX.

            //Read parent instructions
            r = read(output[current][RD],buffer,254);
            buffer[r] = 0;

            //Check what command the parent has issued
            if(strcmp(buffer,"r") == 0){
                printf("%sChild computing random....%s\n",GREEN,DF);
                write(input[current][WR],&random,sizeof(random)); //Send random
            }else if(strcmp(buffer,"i") == 0){
                printf("%sChild sending own pid....%s\n",GREEN,DF);
                pid_t tmp = getpid();
                write(input[current][WR],&tmp,sizeof(tmp)); //Send pid
            }else{
                perr("Command not recognised \n");
            }       
        }
        
    }

}