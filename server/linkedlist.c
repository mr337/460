#include "linkedlist.h"
#include <stdlib.h>

int length = 0;

void linkedListInit()
{
    head = NULL;
}

void addNode(char * msg)
{
    struct node * n;
    n = (struct node *) malloc(sizeof(n));
    n->message = msg;

    if(head == NULL)
    {
        head = n;
    }
    else
    {
        struct node * tmp;
        tmp = (struct node *) malloc(sizeof(tmp));
        tmp = head;
        while(tmp->next != NULL)
        {
            tmp = tmp->next;
        }

        tmp->next = n;
    }

    length++;
}

char * getNode()
{
    return head->message;
}

int nextNode()
{
    if(length > 1)
    {
        head = head->next;
        length--;
        return 0;
    }
    if(length == 1)
    {
        return 1;
    }
    else
    {
        return 1;
    }
}

int isNull()
{
    return length == 0;
}
