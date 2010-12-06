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
int handleACK(ConnectACK * ack);

char ip[15];
char user[UNAMELENGTH+200];
int sock;
long timeConnected;
long keysTyped;
int id;
Chat ch;

int majVersion = 1;
int minVersion = 1;

//for connected stats
time_t tStart, tEnd;


int main(int argc, char * argv[])
{

    //begin connection process
    ConnectInit * cI = (ConnectInit *) malloc(sizeof(ConnectInit));
    initialize_gui();
    echo();

    //gather information
    printw("Please enter a username:  ");
    refresh();
    scanw("%s",user);
    if(strlen(user) > UNAMELENGTH)
    {
        printw("\nUsername too long, max length is:%i\n",UNAMELENGTH);
        refresh();
        getch();
        endwin();
        quit();
        exit(EXIT_SUCCESS);
    }
    strncpy(cI->userName,user,strlen(user));

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
        exit(EXIT_SUCCESS);
    }

    //code to init the server with details
    cI->majorVersion = majVersion;
    cI->minorVersion = minVersion;
    sendConnectInit(cI);
    
    //wait for ConnectACK for id and such
    ConnectACK * ack = (ConnectACK*)malloc(sizeof(ConnectACK));
    getACK(ack);
    printw("Recieved ID: %i  Status:%i\n\n", ack->id, ack->status);
    id = ack->id;

    if(handleACK(ack))
    {//server didn't want client
        printw("Exiting...... Press any key to continue\n");
        refresh();
        getch();
        quit();
        exit(EXIT_SUCCESS);
    }

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

    //start connected timer
    time(&tStart);

    for(;;)
    {
        fd_set tfds = fds;
        struct timeval ttmp;
        ttmp.tv_sec = 0;
        ttmp.tv_usec = t.tv_usec;

                                        //need the timeout for stat
        switch(select(sock+1,&tfds,NULL,NULL,&ttmp))
        {
            case -1:
                printw("Something wrong\n");
                break;
            default:
                if(FD_ISSET(0,&tfds))
                {
                    ch.id = id;
                    keysTyped++;
                    usleep(10000);
                    switch(handle_input(getch()))
                    {
                        case CHAT_QUIT:
                            ch.status=1;
                            strcpy(ch.message,"QUITING");
                            sendChat(&ch);
                            q=1; //bit to quit
                            break;
                        case CHAT_UPDATE:
                            ch.status=2;
                            strncpy(ch.message,message_buffer,MESSAGELENGTH); 
                            sendChat(&ch);
                            break;
                        case CHAT_BROADCAST:
                            ch.status=0;
                            snprintf(ch.message,UNAMELENGTH+MESSAGELENGTH,"%s: %s",cI->userName,message_buffer);
                            sendChat(&ch);
                            break;
                        case CHAT_GAUDY:
                            ch.status=3;
                            snprintf(ch.message,UNAMELENGTH+MESSAGELENGTH,"%s: %s",cI->userName,message_buffer);
                            sendChat(&ch);
                            break;
                        case DS_REQUEST:
                            ch.status=4;
                            strcpy(ch.message, "getlist");
                            sendChat(&ch);
                            break;
                        case DS_VOTE:
                            ch.status=5;
                            sprintf(ch.message, "%i", ds_vote);
                            sendChat(&ch);
                            break;
                        case CHAT_LURK:
                            ch.status=9;
                            sprintf(ch.message, "Lurking...");
                            sendChat(&ch);
                            break;
                        case YELL:
                            ch.status=10;
                            sprintf(ch.message, "yelllist");
                            sendChat(&ch);
                            break;
                        default:
                            //other stuff not defined yet
                            break;
                    }
                }
                if(FD_ISSET(sock,&tfds))
                {
                    if(receiveChat(&ch)) {
                        printw("Error in receiving chat\n");
                        refresh();
                        break;
                    }

                    //printw("Receiving: %s   Status:%i\n",ch.message,ch.status);
                    //refresh();

                    switch(ch.status)
                    {
                        case 0:
                            //printw("Writing to transcript %s\n",ch.message);
                            //refresh();
                            write_to_transcript(ch.message,0);
                            break;
                        case 1:
                            break;
                        case 2:
                            write_to_user_window(ch.id, ch.message);
                            break;
                        case 3:
                            write_to_transcript(ch.message, 1);
                            break;
                        case 4:
                            show_ds_window(ch.message);
                            break;
                        case 5:
                            write_to_transcript(ch.message, 0);
                            break;
                        case 6:
                            show_eject_window(ch.message);
                            break;
                        case 7:
                            //show_timeout_window(ch.message);
                            break;
                        case 8:
                            break;
                        case 9:
                            write_to_user_window(ch.id, ch.message);
                            break;
                        case 10:
                            write_to_transcript(ch.message, 0);
                            break;
                    }
                }


                //print stats
                getStats();
        }

        if(q == 1)
        {
            break;
        }

    }
    free(cI);

    quit();
    exit(EXIT_SUCCESS);
}

int sendReadyChat(char * msg, int status)
{//0 means nothing got sent
    ch.id=id;
    ch.status=status;
    strncpy(ch.message,msg,UNAMELENGTH+MESSAGELENGTH);

    return sendChat(&ch);
}

void getStats() {

    time(&tEnd);
    timeConnected=difftime(tEnd,tStart);
    char buf[240]; //80 witdh * 3 columns
    sprintf(buf, "Time Connected: %ld.%ldm\nSent Traffic:%ldK    Recv Traffic:%ldK\nKeys Typed: %ld", timeConnected/60, ((timeConnected%60)*10)/60, sentBytes/1000, recvBytes/1000,keysTyped);
    write_to_status_window(buf);

    memset(buf,0,240);
    sprintf(buf,"Program: Sienna  Version:%i.%i\nUser Name: %s\nServer IP:%s",majVersion,minVersion,user,ip);
    write_to_program_window(buf);
}

void quit()
{
    endwin();
    closeServer();
}

int handleACK(ConnectACK * ack)
{
    //see networking.h for ACK bit values
    switch(ack->status)
    {
        case 1:
            printw("Too many user connected to server. Please wait and try again.\n");
            return 1;
        case 2:
            printw("The user name is already in use. Please try again with a new user name.\n");
            return 1;
        case 3:
            printw("The server is not properly running correctly. Please contact adminitrator.\n");
            return 1;
        case 4:
            printw("The username is NULL or just spaces. Please correct username and try again.\n");
            return 1;
        default:
            break;
    }

    return 0; //no problems and continue loading client
}
