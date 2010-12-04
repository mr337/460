#include "../networking/networking.h"

#ifndef transcriptlist 
#define transcriptlist 
#define TRANSLEN 23
struct tnode
{
    char message[UNAMELENGTH+MESSAGELENGTH];
    struct tnode *next;
    struct tnode *prev;
};

struct tnode * transhead;
struct tnode * top;
struct tnode * bottom;
char messages[5][81];

void linkedListInit();
void addNode(char * msg);
char * getTop();
char * getBottom();
int scrollUp();
int scrollDown();
int isNull();
int getLength();
int updateTranscript(char * message);

#endif
