#include "linkedlist.h"


void init()
{
    head.next = &end;
}

void addNode(char * msg)
{
    struct node n;
    n.message = msg;
    end.next = &n;
    end = *end.next;
}

char * getNode()
{
    return head.message;
}


void nextNode()
{
    head = *head.next;
}
