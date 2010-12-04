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
#include "../networking/networking.h"
#include "../gui/gui.h"

void quit();
char ip[15];
int sock;
long timeConnected;
long sentTraffic;
long receivedTraffic;
long keysTyped;

int main(int argc, char * argv[])
{

    //if(argc != 3)
    //{
    //    printf("Must be in format program Server Port\n");
    //    exit(EXIT_SUCCESS);
    //}

    ConnectInit * cI = (ConnectInit *) malloc(sizeof(ConnectInit));

    initialize_gui();
    printw("Please enter a username:  ");
    refresh();
    scanw("%s", cI->userName);


    printw("\nPlease enter IP address:  ");
    refresh();
    scanw("%s",ip);


    //connecto to server
    connectToServer(ip,5000);

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

    sock = getSock();

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
                    char c = getch();
                    handle_input(c);
                    
                    
                }
                if(FD_ISSET(sock,&tfds))
                {
                    Chat * ch = (Chat *)malloc(sizeof(Chat));
                    if(!receiveChat()) {
                        printw("Error");
                    }
                    if(ch->status == 
                }
        }

        if(q == 1)
        {
            break;
        }

    }

    quit();
}

void getStats() {

    sprintf(message_buffer, "Time Connected: %ld:%ld\nSent Traffic: %ld\nReceivedTraffic: %ld", timeConnected/60, timeConnected%60, sentTraffic, receivedTraffic);

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
