#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

void getProducerTurn()
{
    // Keep reading the file TURN.txt until it has the value 0
    int num, done=0;
    FILE *turn;
    while(done!=1)
    {
        turn=fopen("TURN.txt","rt");
        num = fgetc(turn);
        if(num=='0')
        {
            done =1;

        }
        fclose(turn);
    }

}


void giveConsumerTurn()
{
    FILE *turn= fopen("TURN.txt","wt");
    fputc('1', turn);
    fclose(turn);
    // Write 1 to TURN.txt to give consumer its turn
}


void producer()
{
    int done = 0;
    char letter;
    FILE *fptr;

    FILE *data=fopen("mydata.txt","rt");
    while(done != 1)
    {
        getProducerTurn();
        letter =fgetc(data);
        if(!feof(data))
        {

            fptr = fopen("DATA.txt", "wt");         //writes one char from mydata.txt to DATA.txt
            fputc(letter, fptr);
            fclose(fptr);
        }
        else
        {
            done =1;
            fptr = fopen("DATA.txt", "wt");
            fputc(0, fptr);
            fclose(fptr);
        }
        giveConsumerTurn();
    }
    fclose(data);
}







