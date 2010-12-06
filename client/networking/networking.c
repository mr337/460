#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <curses.h>
#include "networking.h"

//connStatus, 1 = connected, 0 = disconnected
int connStatus;
int sock;

char tmpSend[1500];
char tmpRecv[1500];

int connectToServer(char * ipAddress, int port)
{
    struct sockaddr_in sAddr;
    memset((void*) &sAddr, 0, sizeof(struct sockaddr_in));
    sAddr.sin_family = AF_INET;
    sAddr.sin_addr.s_addr = INADDR_ANY;
    sAddr.sin_port = 0;

    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    bind(sock, (const struct sockaddr*) &sAddr, sizeof(sAddr));

    sAddr.sin_addr.s_addr = inet_addr(ipAddress);
    sAddr.sin_port = htons(port);

    if(connect(sock, (const struct sockaddr *) &sAddr, sizeof(sAddr)) != 0)
    {
        perror("Error connecting to server");
        return -1;
    }
    else
    {
        connStatus = 1;
    }


    //set vars
    sentBytes=0;
    recvBytes=0;

    return connStatus;
}

int isConnected()
{
    return connStatus;
}

int closeServer()
{
    return close(sock);

}

int getSock()
{
    return sock;
}

int sendConnectInit(ConnectInit * cI)
{
    int tmpSent = send(sock, cI, sizeof(ConnectInit), 0);
    sentBytes += tmpSent;
    return tmpSent;
}

int getACK(ConnectACK * cACK)
{
    int tmpRecv = recv(sock, cACK, sizeof(ConnectACK),0);
    recvBytes += tmpRecv;
    return tmpRecv; 
}

int sendChat(Chat * ch)
{//0 ERROR , 1 good
    memset(tmpSend,0,1500);
    char * sChat = (char*)malloc(1000);
    //int size = serializeChat(sChat, ch);
    int size = serializeChat(tmpSend, ch);
    if(!size)
    {
        printw("Error serializing struct\n");
        free(sChat);
        return 0;
    }
    else
    {//send size of string and them data
        size = snprintf(sChat,size+5+1,"!%4d%s",(int)strlen(tmpSend),tmpSend);
        sentBytes += send(sock,sChat,size,0);
        free(sChat);
        return 1;
    }
}

int receiveChat(Chat * ch)
{
    
    memset(tmpRecv,0,1500);

    if(recv(sock,tmpRecv,5,0)!=5)
    {
        printw("Error receving string size\n");
        refresh();
        return 0;
    }

    if(tmpRecv[0] != '!')
    {
        printw("Recv message did not start with proper marker\n");
        refresh();
        return 0;
    }

    recvBytes += 5;

    int sizeToRecv = atoi(tmpRecv+1);
    memset(tmpRecv,0,1500);
    recvBytes += recv(sock,tmpRecv,sizeToRecv,0);

    deserializeChat(tmpRecv,ch);
    return 0; //no errors    
}

int serializeChat(char * msg, Chat * ch)
{
    //had to increment by 1, strpinf does not count the added \0
    return 1+sprintf(msg,"%i`%i`%i`%s",ch->id,ch->status,ch->messageLen,ch->message);
}

int deserializeChat(char * msg, Chat * ch)
{
    char * delim = strtok(msg,"`");
    ch->id = atoi(delim);
    delim = strtok(NULL,"`");
    ch->status = atoi(delim);
    delim = strtok(NULL,"`");
    ch->messageLen = atoi(delim);
    delim = strtok(NULL,"`");
    strcpy(ch->message,delim);
    return 0; //0 for now errors
}
