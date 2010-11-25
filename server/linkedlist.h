#include "../client/networking/networking.h"

struct node
{
    char * message[MESSAGELENGTH];
    struct node *next;
};

struct node head;
struct node end;


    
