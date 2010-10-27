//header file needed for networking.c
//

int connectToServer(char *, int);
int isConnected();
int closeServer();
int sendMessage(char *);
void receiveMessage(char *);
