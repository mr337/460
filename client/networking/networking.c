#include <stdio.h>
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

int sendConnectInit(ConnectInit * cI)
{
    return send(sock, cI, sizeof(ConnectInit), 0);
}

int getACK(ConnectACK * cACK)
{
    return recv(sock, cACK, sizeof(ConnectACK),0);
}

int sendChat(Chat * ch)
{
    char * lala = &ch->message;
    printf("Sending:%s\n", lala);
    return send(sock,ch, sizeof(ch),0);
}

int receiveChat(Chat * ch)
{
    return recv(sock, ch, sizeof(Chat),MSG_DONTWAIT);
}
