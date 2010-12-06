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
        //tmp = (struct tnode *) malloc(sizeof(struct tnode));
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

int updateTranscript(char *message)
{
    int i;
    int lastSpace = 0;
    int lastBreak = 0;
    int msg_len = strlen(message);
    char *line = messages[0];
    int message_count = 0;

    line[0] = '\0';
    message_count = 0;

    for ( i = 0; i <= msg_len; i++ ) {
        if ( msg_len > 0 ) {
            if ( message[i] == ' ' ) {
              lastSpace = i;
            }

            line[i - lastBreak] = message[i];
        }

        if ( i == msg_len - 1  ) {
            line[i+1] = '\0';
            addNode(line);
            message_count++;
            line = messages[message_count];
            lastBreak = i;
            lastSpace = i;
        }
        else if ( (i - lastBreak) == 39 ) {
            if (lastSpace > lastBreak) {
                line[lastSpace - lastBreak] = '\0';
                lastBreak = lastSpace + 1;
                i = lastSpace;
            } else {
                line[40] = '\0';
                lastBreak = i + 1;
                lastSpace = i;
            }
            addNode(line);
            message_count++;
            line = messages[message_count];
        }       
    }
    return message_count;
}
