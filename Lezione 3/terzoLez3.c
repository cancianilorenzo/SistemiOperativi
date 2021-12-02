#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

char *stringRev(char *str){
    int lunghezza = strlen(str);
    char stringaRev[lunghezza];
    //printf("%i", lunghezza);
    int posAttuale = 0;
    for(int i = lunghezza-1; i > -1; i--){
        //printf("%i\n", i);
        stringaRev[posAttuale] = str[i];
        //printf(" str --> %c\n", str[i]);
        //printf("stringaRev --> %c\n", stringaRev[posAttuale]);
        posAttuale++;
    }
    stringaRev[lunghezza] = '\0';
    strcpy(str,stringaRev);
    printf("%s\n", stringaRev);
    return *str;
}

int main(int argc, char** argv){
    
    //printf("%s", argv[1]);
    //stringRev(argv[1]);
    //printf("%s\n", stringRev(argv[1]));

    return 0;
}