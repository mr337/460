#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include "networking.c"

void child_func(int childnum);

int main(int argc, char * argv[])
{
    int nchildren;
    int pid;
    int x;

    if (argc > 1)
    {
        nchildren = atoi(argv[1]);
    }

    for(x = 0; x < nchildren; x++)
    {
        sleep(1);
        if((pid = fork()) == 0)
        {
            child_func(x+1);
            return 0;
        }
    }


    wait(NULL);
    return 0;

}

void child_func(int childnum)
{

    //connecto to server
    connectToServer("127.0.0.1",5000);
    if(isConnected()!= 1)
    {
        perror("Error connecting to client\n");
    }


    //code to init the server with details
    ConnectInit cI;
    char * name = &cI.userName;
    strcpy(name,"Lee\0");    
    cI.majorVersion = 1;
    cI.minorVersion = 9;
    sendConnectInit(&cI);

    //wait for ConnectACK for id and such
    sleep(1);
    ConnectACK* ack = getACK();
    printf("Recieved ID: %i  Status:%i\n", ack->id, ack->status);

    closeServer();

}
