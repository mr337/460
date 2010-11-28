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

void quit();

int main(int argc, char * argv[])
{
    initscr();
    noecho();
    cbreak();
    refresh();

    //connecto to server
    connectToServer("127.0.0.1",5000);

    if(isConnected()!= 1)
    {
       printw("Error connecting to client\n");
       refresh();
       quit();
    }


    printw("Connecting with username: %s\n", argv[1]);

    //code to init the server with details
    ConnectInit * cI = (ConnectInit *) malloc(sizeof(ConnectInit));
    char * name = &cI->userName;
    strcpy(name, argv[1]);
    cI->majorVersion = 1;
    cI->minorVersion = 9;
    sendConnectInit(cI);
    free(cI);


    //wait for ConnectACK for id and such
    sleep(1);
    ConnectACK * ack = (ConnectACK*)malloc(sizeof(ConnectACK));
    getACK(ack);
    printw("Recieved ID: %i  Status:%i\n", ack->id, ack->status);
    int id = ack->id;
    free(ack);
    refresh();

    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(0,&fds);

    struct timeval t;
    t.tv_sec = 0;
    t.tv_usec = 500000;
    
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
                    Chat * ch = (Chat *)malloc(sizeof(Chat));
                    ch->id = id;
                    ch->status = 0;
                    ch->messageLen = 1;
                    char * message = &ch->message;
                    strcpy(message,&c); 
                    //strcpy(msg, &c);
                    //strcat(msg, "\0");
                    //strcpy(ch->message, msg);
                    if(!sendChat(ch))
                    {
                        printw("Error sending chat\n");
                    }
                    refresh();
                }

                Chat * ch = (Chat *)malloc(sizeof(Chat));
                if(!receiveChat(ch))
                {
                    printw("%s\n",ch->message);
                    refresh();
                }
                free(ch);
        }

        if(q == 1)
        {
            break;
        }

    }

    quit();
}

void quit()
{
    Chat * ch = (Chat *)malloc(sizeof(Chat));
    ch->id = 0;
    ch->status = 1; //disconnect bit
    sendChat(ch);
    endwin();
    closeServer();
    exit(EXIT_SUCCESS);
}
