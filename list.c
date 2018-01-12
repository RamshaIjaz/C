#include <stdio.h>
#include <stdlib.h>
#include "list.h"

struct NODE *head;          //private global head pointer

void newList()
{
    head = NULL;
}

//created newNode in reverse order
int addNode (int value)
{
    struct NODE* temp = (struct NODE *) malloc(sizeof(struct NODE));
    if(temp == NULL) return EXIT_FAILURE;           //terminated if temp is NULL
    temp-> data = value;
    temp->next = head;
    head = temp;
    return EXIT_SUCCESS;
}

//prints the list via traversing through the list
void prettyPrint()
{
    struct NODE *ptr = head;
    while (ptr!=NULL)
    {
        printf("%d ", ptr->data);
        ptr=ptr->next;
    }

}
