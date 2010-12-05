#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
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
int sendReadyChat(char * msg, int status);
void getStats();

char ip[15];
int sock;
long timeConnected;
long sentTraffic;
long receivedTraffic;
long keysTyped;
int id;
Chat ch;

void count(int sig) {
    timeConnected++;
    getStats();
}

int main(int argc, char * argv[])
{

    //if(argc != 3)
    //{
    //    printf("Must be in format program Server Port\n");
    //    exit(EXIT_SUCCESS);
    //}

    ConnectInit * cI = (ConnectInit *) malloc(sizeof(ConnectInit));

    initialize_gui();
    echo();
    printw("Please enter a username:  ");
    refresh();
    scanw("%s", cI->userName);


    printw("\nPlease enter IP address:  ");
    refresh();
    scanw("%s",ip);

    printw("Connecting.....\n");
    refresh();


    //connecto to server
    connectToServer(ip,5000);

    if(isConnected()!= 1)
    {
        printw("\nFailed to connect to server.\n");
        refresh();
        getch();
        endwin();
        quit();
    }


    printw("Submitting username: %s\n", argv[1]);

    //code to init the server with details
    cI->majorVersion = 1;
    cI->minorVersion = 9;
    sendConnectInit(cI);
    


    //wait for ConnectACK for id and such
    sleep(1);
    ConnectACK * ack = (ConnectACK*)malloc(sizeof(ConnectACK));
    getACK(ack);
    printw("Recieved ID: %i  Status:%i\n", ack->id, ack->status);
    id = ack->id;
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
    noecho();
    initialize_windows();

    for(;;)
    {
        signal(SIGALRM, count);

        errno = 0;
        ualarm(1000, 1000);

        fd_set tfds = fds;
        struct timeval ttmp;
        ttmp.tv_sec = 0;
        ttmp.tv_usec = t.tv_usec;

        switch(pselect(sock+1,&tfds,NULL,NULL,NULL,NULL))
        {
            case -1:
                printw("Something wrong\n");
                break;
            default:
                if(FD_ISSET(0,&tfds))
                {
                    Chat * ch = (Chat *)malloc(sizeof(Chat));
                    char c = getch();
                    int i = handle_input(c);
                    ch->id = id;
                    strcpy(ch->message, *cI->userName);
                    if(i == 1) {
                        ch->status = 1;
                    } else if(i == 2) {
                        ch->status = 0;
                        strcat(ch->message, message_buffer);
                        strcpy(message_buffer, "\0");
                    } else {
                        ch->status = 2;
                        strcpy(ch->message, &c);
                    }
                    sendChat(ch);
                }
                if(FD_ISSET(sock,&tfds))
                {
                    Chat * ch = (Chat *)malloc(sizeof(Chat));
                    if(!receiveChat(ch)) {
                        printw("Error");
                    }
                    if(ch->status == 0) {
                        write_to_transcript(ch->message, 0);
                    }else if(ch->status == 1) {
                        
                    }else if(ch->status == 2) {
                        write_to_user_window(ch->id, ch->message);
                    }else if(ch->status == 3) {
                        write_to_transcript(ch->message, 0);
                    }
                }
        }

        if(q == 1)
        {
            break;
        }

    }
    free(cI);

    quit();
}
int sendReadyChat(char * msg, int status)
{//0 means nothing got sent
    ch.id=id;
    ch.status=status;
    strncpy(ch.message,msg,UNAMELENGTH+MESSAGELENGTH);

    return sendChat(&ch);
}

void getStats() {
    char * stats;
    sprintf(stats, "Time Connected: %ld:%ld\nSent Traffic: %ld\nReceivedTraffic: %ld", timeConnected/60, timeConnected%60, sentTraffic, receivedTraffic);
    write_to_status_window(stats);
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
