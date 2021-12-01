#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char** argv){
    
    char letto;
    int lunghezza = 0;
    /*read(0, &letto, 1);

    while(letto != '\0'){
        lunghezza++;
        read(0, &letto, 1);
    }*/

    /*
    Funziona ma non conta spazi!!!
    for(int j=1;j<argc;j++){
        for(int i =0; argv[j][i]!='\0';i++){
            lunghezza++;
        }
    }
    */

    for(lunghezza = 0; argv[1][lunghezza] != '\0'; lunghezza++){
        //printf("Contato %i\n", lunghezza);
    }

    printf("La stringa ha lunghezza %i\n", lunghezza);

    return 0;
}