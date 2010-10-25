#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include "client.h"

//connStatus, 1 = connected, 0 = disconnected
int connStatus;
int sock;


int connectToServer(char * ipAddress, int port)
{
    struct sockaddr_in sAddr;
    memset((void*) &sAddr, 0, sizeof(struct sockaddr_in));
    sAddr.sin_family = AF_INET;
    sAddr.sin_addr.s_addr = ANADDR_ANY;
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


}

int isConnected()
{
    return connStatus;
}

int closeServer();
{
    return close(sock);

}

int sendMessage(char * message)
{//this is only for testing
    return send(sock, message, strlen(message));
}

char * receiveMessage()
{//this is only for testing
    char rec[25];
    recv(sock, rec, 25,0);
    return &rec;
}


