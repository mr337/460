//header file needed for networking.c
//

typedef struct
{
    char * userName[30];
    int majorVersion;
    int minorVersion;
    //maybe add sAddr so we can read that for the address of server
}ConnectInit;

typedef struct
{
    int stats; //see notes below
    int id; //this is the id the server will give to client
}ConnectACK;

typedef struct
{
    int id;
    int status; //used for lark, yelp, gauntlet - see project notes
    int messageLen; //message length for reading
    char * message;
}Chat;

int connectToServer(char *, int);
int isConnected();
int closeServer();
int sendMessage(char *);
void receiveMessage(char *);
int sendConnectInit(ConnectInit *);


//ConnectACK notes
//status bits: 0 - no erros, connection working
//             1 - too many conencted users, please try again
//             2 - username in use, change and reconnect
