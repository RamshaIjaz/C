#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "Declaration.h"


void main()
{

    FILE *turn = fopen("TURN.txt", "wt");
    fputc('0',turn);
    fclose(turn);

    int pid = fork();       //fork for producer



    if(pid <0 )
    {
        printf("Forking failed\n");
        exit(-1);
    }
    if(pid == 0)
    {
        producer();
    }
    else
    {

            consumer();

    }
    exit(0);
}

