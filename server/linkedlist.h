
struct node
{
    char * message;
    struct node *next;
};

struct node * head;

void linkedListInit();
void addNode(char * msg);
char * getNode();
int nextNode();
int isNull();
