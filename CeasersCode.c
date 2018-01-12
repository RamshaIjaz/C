#include <stdio.h>
#include <stdlib.h>

int main ()
{
    char o_message [100];
    char e_message [100];
    char d_message [100];
    int key,i=0,c,newVal,shLeft,aftShVal,shRight;

    printf("Enter your sentence ");
    scanf ("%[^\n]%*c", o_message);
    printf("Sentence: %s\n",o_message);
    printf("Enter your key ");
    scanf("%i", &key);
    printf("Key: %i\n", key);


    while(o_message[i]!= '\0')
    {
        c=o_message[i];                     //c is the ASCII value of the character in the string
        if(c==32)                           //checks for space and continues the loop and stores in e_message
        {
            e_message[i]=c;
            i++;
            continue;
        }
        else if(c>=65 && c <=90)                 //checks for captial letters
        {
            newVal=c-65;                    //converts ASCII to A=0,B=1...
            shLeft=newVal-key;              //shifts new value of char to left by subtracting the key
            if (shLeft<0)
            {
                aftShVal=26-(abs(shLeft)%26); //if new value is < 0 it goes backwards i.e gets the values from 25 onwards order
            }
            else
            {
                aftShVal=shLeft%26;
            }
            aftShVal=aftShVal+65;
            e_message[i]=aftShVal;
        }
        else if (c>=97 && c<=122)               //checks for small letters
        {
            newVal=c-97;
            shLeft=newVal-key;
            if (shLeft<0)
            {
                aftShVal=26-(abs(shLeft)%26);
            }
            else
            {
                aftShVal=shLeft%26;
            }
            aftShVal=aftShVal+97;
            e_message[i]=aftShVal;
        }

        i++;

    }
    e_message[i]='\0';                          //adds a Null terminator
    i=0;
    while(e_message[i]!= '\0')
    {
        c=e_message[i];                     //c is the ASCII value of the encrypted character in the string
        if(c==32)                           //checks for space and stores it in d_message
        {
            d_message[i]=c;
            i++;
            continue;
        }
        else if(c>=65 && c <=90)             //checks for captial letters
        {
            newVal=c-65;                    //converts ASCII to A=0,B=1...
            shRight=newVal+key;              //shifts encrypted value of char to right by adding the key
            aftShVal=shRight%26;

            aftShVal=aftShVal+65;
            d_message[i]=aftShVal;
        }
        else if (c>=97 && c<=122)               //checks for small letters
        {
            newVal=c-97;
            shRight=newVal+key;

            aftShVal=shRight%26;

            aftShVal=aftShVal+97;
            d_message[i]=aftShVal;
        }
        i++;
    }
    d_message[i]='\0';                          //adds a Null terminator
    printf("Encrypted message: %s\n",e_message); //prints the encrypted message
    printf("Decrypted message: %s\n",d_message);     //prints the decrypted message
    return 0;
}

