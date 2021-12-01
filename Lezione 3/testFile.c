#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char ** argv){

    int fd = open("argc.txt", O_CREAT|O_RDWR, S_IRUSR|S_IWUSR);
    printf("%i\n", argc);

    for(int i = 1; i < argc; i++){
        write(fd, argv[i], strlen(argv[i]));
        write(fd, " ", 1);
    }
    write(fd, "\n", strlen("\n"));

    

    
    /*int newFile = creat("name.txt", S_IRUSR|S_IWUSR); //Crea
    int newFile2 = open("name.txt", O_CREAT|O_RDWR, S_IRUSR|S_IWUSR);

    char content[20];
    int canRead;

    write(newFile2, "ciao a tutti\n", strlen("ciao a tutti\n"));

    canRead = read(newFile2, content, 20);
    while(canRead>0){
        printf("%s\n", content);
        canRead = read(newFile2, content, 20);
    }
    
    //close(newFile);
    close(newFile2);*/
    close(fd);

    return 0;
}