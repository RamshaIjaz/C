#include <stdio.h>
#include <stdlib.h>
#include "list.h"

int main()
{
    int n, i;
    newList();

//asks for user-input until negative integer is entered
    do
    {
        printf("Enter a number\n");     //asks for user-input
        scanf("%i", &n);
        if(n>0)
        {
            i=addNode(n);
        }
    }
    while(n>0);
    prettyPrint();
    return 0;
}
