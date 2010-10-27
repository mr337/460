//header file needed for networking.c
//

int connectToServer(char *, int);
int isConnected();
int closeServer();
int sendMessage(char *);
void receiveMessage(char *);

typedef struct
{
    char * userName[30];
    int majorVerion;
    int minorVersion;
    //maybe add sAddr so we can read that for the address of server
}ConnectInit;

typedef struct
{
    int stats;
    int id; //this is the id the server will give to client
}ConnectACK;

typedef struct
{
    int id;
    int status; //used for lark, yelp, gauntlet - see project notes
    int messageLen; //message length for reading
    char * message;
}Chat;
