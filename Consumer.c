#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

void getConsumerTurn()
{

    // Keep reading the file TURN.txt until it has the value 1
    int num, done=0;
    FILE *turn;
    while(done!=1)
    {
        turn=fopen("TURN.txt","rt");
        num = fgetc(turn);
        if(num=='1')
        {
            done =1;

        }
        fclose(turn);
    }

}

void giveProducerTurn()
{


    FILE *turn= fopen("TURN.txt","wt");
    fputc('0', turn);
    fclose(turn);
    // Write 0 to TURN.txt
}

void consumer()
{
    int done = 0;

    FILE *data;
    char character;
    while(done != 1)
    {
        getConsumerTurn();
        data = fopen("DATA.txt","rt");
        character = fgetc(data);
        if(character!=0)
        {
            printf("%c", character);
        }
        else
        {
            done =1;
        }
        fclose(data);


        giveProducerTurn();
    }

}
