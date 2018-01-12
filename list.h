#ifndef LISTH
#define LISTH

void newList();
int addNode (int);
void prettyPrint();

struct NODE
{
    int data;
    struct NODE *next;
};


#endif // LIST_H_INCLUDED
