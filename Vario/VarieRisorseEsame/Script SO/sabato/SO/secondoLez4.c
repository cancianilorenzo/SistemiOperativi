/*
Creo fd
dup2
Fork
execv
1 -> out
2 -> err
*/

#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(int argc, char **argv)
{

    int processi = argc - 1;

    int fdout = open("output.txt", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    int fderr = open("output.txt", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    dup2(fdout, 1);
    dup2(fderr, 2);

    //printf("%i\n", processi);

    for (int i = 1; i < processi + 1; i++)
    {
        int figlio = fork();
        if (figlio == 0)
        {
            //sono nel figlio
            execv(argv[i], argv);
            return 0;
        }
    }
while(wait(NULL)>0); //Aspetta la fine di tutti i figli
    return 0;
}