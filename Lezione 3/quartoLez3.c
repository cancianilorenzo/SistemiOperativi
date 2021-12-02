#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int stringpos(char *str, char chr){
    //int lunghezza = strlen(str); Non sapendo com'è scritta non so se ottimiz.
    for(int i = 0;  str[i] != '\0'; i++){
        if(str[i] == chr){
            return ++i;
        }
    }
}

int main (int argc, char ** argv){
    
    int posizione = stringpos(argv[1], argv[2][0]);
    printf("%i\n", posizione);

    return 0;
}