//header file needed for networking.c

#ifndef networking
#define networking

#define UNAMELENGTH 30 //max username length
#define MESSAGELENGTH 255 //max message lengh in chars

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
    int id; //if not set to 0 then means it's a keyboard update (10 people)
    int status; //used for lark, yelp, gauntlet - see project notes
    int messageLen; //message length for reading
    char message[UNAMELENGTH+MESSAGELENGTH]; //message is as long as messagelength, but when resending server
                                               //will attach username 
}Chat;

int connectToServer(char *, int);
int isConnected();
int closeServer();
int sendMessage(char *);
void receiveMessage(char *);
int sendConnectInit(ConnectInit *);
int sendConnectInit(ConnectInit * cI);
int getACK(ConnectACK * cI);
int sendChat(Chat * ch);
int receiveChat(Chat * ch);


//ConnectACK notes
//status bits: 0 - no erros, connection working
//             1 - too many conencted users, please try again
//             2 - username in use, change and reconnect
//             3 - something wrong with server - failed when getting user ID
//             4 - username cannot be NULL


//Chat notes
//status bits: 0 - nothing unusual
//status bits: 1 - disconnecting...

#endif
