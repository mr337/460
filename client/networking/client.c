#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include "networking.c"

void child_func(int childnum);

int main(int argc, char * argv[])
{
    int nchildren = 20000;
    int pid;
    int x;

    if (argc > 1)
    {
        nchildren = atoi(argv[1]);
    }

    for(x = 0; x < nchildren; x++)
    {
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
    char buffer[25];

    connectToServer("127.0.0.1",5000);
    if(isConnected()!= 1)
    {
        perror("Error connecting to client\n");
    }

    
    snprintf(buffer, 128, "%i", childnum);
    sleep(1);
    printf("child #%i sent %i chars\n", childnum, sendMessage(buffer));
    sleep(1);

    char buf[25];
    receiveMessage(buf);
    printf("child #%i recieved %s \n", childnum, buf);
    sleep(1);

    closeServer();

}
