
struct node
{
    char * message;
    struct node *next;
};

struct node head;
struct node end;

void init();
void addNode(char * msg);
char * getNode();
void nextNode();
    
