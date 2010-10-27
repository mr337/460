#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <string.h>

void* thread_proc(void *arg);

int maxUsers;
int numUsers;

int main(int argc, char *argv[])
{

    if(argc != 3)
    {
        printf("Must be in format: server port Max_Clients");
    }

    //get info from CL
    int port = atoi(argv[1]);
    maxUsers = atoi(argv[2]);

    if(port == 0 || maxUsers == 0)
    {
        perror("Port or MaxUsers must be greater than 0");
    }
    

    //print stats
    printf("Starting server\nPort: %i\nMax Users of: %i\n", port, maxUsers);


    struct sockaddr_in sAddr;
    int listensock;
    int newsock;
    int result;
    pthread_t thread_id;
    int val;

    listensock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    val = 1;
    result = setsockopt(listensock, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));
    if(result < 0)
    {
        perror("server");
        return 0;
    }


    sAddr.sin_family = AF_INET;
    sAddr.sin_port = htons(port);
    sAddr.sin_addr.s_addr = INADDR_ANY;

    result = bind(listensock, (struct sockaddr*) &sAddr, sizeof(sAddr));

    if(result < 0)
    {
        perror("server");
        return 0;
    }

    result = listen(listensock, 10);

    if(result < 0)
    {
        perror("server");
        return 0;
    }

    while(1)
    {
        printf("Server now waiting for client.....\n");
        newsock = accept(listensock, NULL, NULL);

        result = pthread_create(&thread_id, NULL, thread_proc, (void*) newsock);
        if(result != 0)
        {
            perror("server");
        }

        numUsers++;

        pthread_detach(thread_id);
        sched_yield();
    }

}

void* thread_proc(void *arg)
{
    int sock;
    char buffer[25];
    char response[25] = "How are you string: ";
    int nread;

    printf("child thread %i started with pid %i\n", pthread_self(), getpid());

    sock = (int) arg;

    if(numUsers > maxUsers)
    {//to many users
    }


    nread = recv(sock, buffer, 25, 0);

    buffer[nread] = '\0';
    strcat(response, buffer);
    printf("%s\n", buffer);
    send(sock, response, strlen(response), 0);
    close(sock);
    numUsers--;

    printf("child thred %i finished with pid %i\n", pthread_self(), getpid());
}
