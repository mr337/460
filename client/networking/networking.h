//header file needed for networking.c
//

#define UNAMELENGTH 30
#define MESSAGELENGTH 255

typedef struct
{
    char * userName[UNAMELENGTH];
    int majorVersion;
    int minorVersion;
    //maybe add sAddr so we can read that for the address of server
}ConnectInit;

typedef struct
{
    int status; //see notes below
    int id; //this is the id the server will give to client
}ConnectACK;

typedef struct
{
    int id;
    int status; //used for lark, yelp, gauntlet - see project notes
    int messageLen; //message length for reading
    char message[MESSAGELENGHT];
}Chat;

int connectToServer(char *, int);
int isConnected();
int closeServer();
int sendMessage(char *);
void receiveMessage(char *);
int sendConnectInit(ConnectInit *);
struct ConnectAck * getACK();


//ConnectACK notes
//status bits: 0 - no erros, connection working
//             1 - too many conencted users, please try again
//             2 - username in use, change and reconnect
//             3 - something wrong with server - failed when getting user ID
//             4 - username cannot be NULL
