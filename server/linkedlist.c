#include "linkedlist.h"
#include <stdlib.h>
#include <string.h>

int length = 0;

void linkedListInit()
{
    head = NULL;
}

void addNode(char * msg)
{
    struct node * n;
    n = (struct node *) malloc(sizeof(struct node));
    strcpy(n->message,msg);

    if(head == NULL)
    {
        head = n;
    }
    else
    {
        struct node * tmp;
        tmp = (struct node *) malloc(sizeof(struct node));
        tmp = head;
        while(tmp->next != NULL)
        {
            //printf("LL %s\n",tmp->message);
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
        //TODO need to free up the memory from the old node
        //struct node * old = head;
        struct node * old = &head;
        head = head->next;
        length--;
        free(old);
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

int getLength()
{
    return length;
}
