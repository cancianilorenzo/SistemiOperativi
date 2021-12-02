/*
*Memorizza parametri corretti
*errore in caso di paramametri scorretti
*carattere c per terminare programma
*prima della chiusura stampa array scelte corrette
*/
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char **argv)
{

    char letto;
    char buffer[10];
    int posBuffer = 0;

    while (letto != 'c')
    {
        read(0, &letto, 1);

        switch (letto)
        {
            case 'h':
            buffer[posBuffer] = 'h';
            posBuffer++;
            break;
            case 's':
            buffer[posBuffer] = 's';
            posBuffer++;
            break;
            case 'f':
            buffer[posBuffer] = 'f';
            posBuffer++;
            break;
            default: 
            printf("Errore nel parsing dei comandi")
        }
    }

    for(int i = 0; i < posBuffer; i++){
        printf("%c ", buffer[i]);
    }
    printf("\n");

    return 0;
}