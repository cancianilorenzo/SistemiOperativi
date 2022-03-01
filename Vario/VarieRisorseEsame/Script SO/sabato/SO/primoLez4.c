#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int figlio;

int main(int argc, char **argv)
{

    char* primoBinario = argv[1];
    char* secondoBinartio = argv[2];

    //execv("./execv2.out",argv);
    for (int i = 1; i < 3; i++)
    {
        int figlio = fork();
        if (figlio == 0)
        {
            //I'm the child
            execv(argv[i], argv);
        }
    }
    while(wait(NULL)>0);
    return 0;
}