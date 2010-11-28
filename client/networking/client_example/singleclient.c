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
#include "../networking.h"

void quit();

int main(int argc, char * argv[])
{

    if(argc != 4)
    {
        printf("Must be in format program Username Server Port\n");
        exit(EXIT_SUCCESS);
    }
    initscr();
    noecho();
    cbreak();
    refresh();

    //connecto to server
    connectToServer(argv[2],atoi(argv[3]));

    if(isConnected()!= 1)
    {
       printw("Error connecting to client\n");
       refresh();
       getch();
       endwin();
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

    int sock = getSock();

    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(0,&fds);
    FD_SET(sock,&fds);

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

        switch(select(sock+1,&tfds,NULL,NULL,&ttmp))
        {
            case -1:
                printw("Something wrong\n");
                break;
            default:
                if(FD_ISSET(0,&tfds))
                {
                    Chat * ch = (Chat *)malloc(sizeof(Chat));
                    ch->id = id;
                    ch->status = 0;

                    //have to manualy add the User: message
                    //unless it's an update, chat bit = 2
                    char * tmp = malloc(UNAMELENGTH+MESSAGELENGTH);

                    getstr(tmp);
                    sprintf(ch->message,"%s: %s",argv[1],tmp);
                    ch->messageLen = strlen(ch->message);
                    //printw("STDIN:%s\n",ch->message);
                    if(!strcmp(tmp, "q"))
                    {
                        ch->status = 1;
                        q = 1;
                    }

                    if(!sendChat(ch))
                    {
                        printw("Error sending chat\n");
                    }
                    refresh();

                    free(tmp);
                }
                if(FD_ISSET(sock,&tfds))
                {
                    Chat * ch = (Chat *)malloc(sizeof(Chat));
                    if(!receiveChat(ch))
                    {
                        printw("%s\n",ch->message);
                        refresh();
                    }
                    free(ch);
                }
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
