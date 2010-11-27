#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/times.h>
#include <netinet/in.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <curses.h>
#include "networking.h"


int main(int argc, char * argv[])
{
    //connecto to server
    connectToServer("127.0.0.1",5000);

    if(isConnected()!= 1)
    {
        perror("Error connecting to client\n");
    }

    initscr();
    noecho();
    cbreak();
    refresh();


    //code to init the server with details
    ConnectInit * cI; 
    cI = (ConnectInit *) malloc(sizeof(ConnectInit));
    sprintf(*cI->userName,"%s",argv[1]); 
    cI->majorVersion = 1;
    cI->minorVersion = 9;
    sendConnectInit(cI);
    free(cI);


    //wait for ConnectACK for id and such
    sleep(1);
    ConnectACK * ack = (ConnectACK*)malloc(sizeof(ConnectACK));
    getACK(ack);
    printw("Recieved ID: %i  Status:%i\n", ack->id, ack->status);
    free(ack);
    refresh();

    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(0,&fds);

    struct timeval t;
    t.tv_sec = 0;
    t.tv_usec = 500;
    
    int q = 0;

    for(;;)
    {
        fd_set tfds = fds;
        struct timeval ttmp;
        ttmp.tv_sec = 0;
        ttmp.tv_usec = t.tv_usec;

        switch(select(1,&tfds,NULL,NULL,&ttmp))
        {
            case -1:
                printw("Something wrong\n");
                break;
            default:
                if(FD_ISSET(0,&tfds))
                {
                    char c = getch();
                    if(c == 'q')
                    {
                        q = 1;
                    }
                    printw("STDIN:%c\n",c); 
                    refresh();
                }

                Chat * ch = (Chat *)malloc(sizeof(Chat));
                receiveChat(ch);
                printw("%s\n",ch->message);
                refresh();
                free(ch);
        }

        if(q == 1)
        {
            break;
        }

    }

    closeServer();
    exit(EXIT_SUCCESS);
}
