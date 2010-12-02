#include "transcriptlist.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int length = 0;
int locked = 0;

void linkedListInit()
{
    transhead = NULL;

}

void addNode(char * msg)
{
    struct tnode * n;
    n = (struct tnode *) malloc(sizeof(struct tnode));
    strcpy(n->message,msg);

    if(transhead == NULL)
    {
        transhead = n;
        top = n;
        bottom = n;
    }
    else
    {
        struct tnode * tmp;
        tmp = (struct tnode *) malloc(sizeof(struct tnode));
        tmp = transhead;
        while(tmp->next != NULL)
        {
            //printf("LL %s\n",tmp->message);
            tmp = tmp->next;
        }

        tmp->next = n;
        n->prev = tmp;
        bottom = n;
    }

    length++;

    if (length > TRANSLEN) {
        top = top->next;
    }
}

char * getTop()
{
    return top->message;
}

char * getBottom()
{
    return bottom->message;
}

int scrollDown()
{
    if(length > TRANSLEN && bottom->next != NULL)
    {
        //TODO: start timer and lock window
        top = top->next;
        bottom = bottom->next;
        return 1;
    }
    else
    {
        return 0;
    }
}

int scrollUp()
{
    if(length > TRANSLEN && top != transhead)
    {
        //TODO: start timer and lock window
        top = top->prev;
        bottom = bottom->prev;
        return 1;
    }
    else
    {
        return 0;
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

char * updateTranscript(char *message)
{
    int i;
    int lastSpace = 0;
    int lastBreak = 0;
    int msg_len = strlen(message);
    char line[81];

    line[0] = '\0';

    for ( i = 0; i <= msg_len; i++ ) {
        if ( msg_len > 0 ) {
            if ( message[i] == ' ' ) {
              lastSpace = i;
            }

            line[i] = message[lastBreak  + i];
        }

        if ( i == msg_len  ) {
            line[i+1] = '\0';
            addNode(line);
        }
        else if ( (i - lastBreak) == 79 ) {
            if (lastSpace > lastBreak) {
                line[lastSpace - lastBreak] = '\0';
                lastBreak = lastSpace;
                i = lastSpace;
            } else {
                line[80] = '\0';
                lastBreak += 79;
                lastSpace += 79;
            }
            addNode(line);
        }        
    }

}


