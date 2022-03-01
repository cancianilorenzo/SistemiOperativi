#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

//./eseguibile <comando1> <comando2>
//equivale a ls | wc

int main(int argc, char **argv){
    int file;

    file = open ("file.txt", O_RDWR);

    int bit_lettura; char stringa[10]; int num_bit=10;
   
        bit_lettura= read(file, stringa, num_bit);
        printf("%d\n", bit_lettura);
        int valore;
        valore = lseek(file, 0, SEEK_SET);
        printf("%d\n", valore);

        valore = lseek(file, 0, SEEK_CUR);
        printf("%d\n", valore);       
        valore = lseek(file, 0, SEEK_END);
        printf("%d\n", valore);


    return 0;
}