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

char opt[2];
char filename[50];

int parseargs(int nargs, char **cargs)
{
    int curarg = 1; //argv[0] == ./program !!
    char arg[30];
    int oi = 0;
    bool optc = false;
    bool optl = false;
    bool optfn = false;
    while (curarg < nargs){
        strcpy(arg, cargs[curarg]);
        if (strcmp(arg, "-c") == 0 && !optc){
            opt[oi++] = 'c';
            optc = false;
        } //setto se c
        if (strcmp(arg, "-l") == 0 && !optc){
            opt[oi++] = 'l';
            optc = false;
        } //setto se l
        if (strcmp(arg, "-c") != 0 && strcmp(arg, "-l") != 0 && arg[0] == '-')
            exit(1);
        if (strcmp(arg, "-c") != 0 && strcmp(arg, "-l") != 0 && arg[0] != '-'){
            if (!optfn){
                strcpy(filename, arg);
                optfn = true;
            }else exit(2);
        };
        if (oi > 2) exit(3);
        curarg++;
    };
printf("opt0 --> %c\n", opt[0]);
printf("opt1 --> %c\n", opt[1]);
printf("filename --> %s\n", filename);
    return 0;
}

int main(int argc, char **argv){
    parseargs(argc, argv);
    return 0;
}