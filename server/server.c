#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <string.h>
#include <semaphore.h>
#include "../client/networking/networking.h"

void* thread_proc(void *arg);

int maxUsers;
int numUsers;
int totalUsers;
char **userNames;
sem_t users;

int main(int argc, char *argv[])
{

    char master_array[maxUsers];

    if(argc != 3)
    {
        printf("Must be in format: server port Max_Clients\n");
        exit(EXIT_SUCCESS);
    }


    //get info from CL
    int port = atoi(argv[1]);
    maxUsers = atoi(argv[2]);

    if(port == 0 || maxUsers == 0)
    {
        perror("Port or MaxUsers must be greater than 0");
    }

    //init 
    sem_init(&users,0,1);
    numUsers = 0;
    totalUsers = 0;

    //init of array of strings
    userNames = malloc(maxUsers * sizeof(char *));
    int i = 0;
    for(i=0; i< maxUsers; i++)
    {
        userNames[i] = malloc(UNAMELENGTH * sizeof(char));
        userNames[i] = "\0";
    }


    //print stats
    printf("Starting server\nPort: %i\nMax Users of: %i\nMax UserName Length: %i\n", port, maxUsers, UNAMELENGTH);

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
        //printf("SERVER: Now Accepting Clients \n");

        newsock = accept(listensock, NULL, NULL);

        result = pthread_create(&thread_id, NULL, thread_proc, (void*) newsock);
        if(result != 0)
        {
            perror("server");
        }


        pthread_detach(thread_id);
        sched_yield();
    }

}

void* thread_proc(void *arg)
{
    int sock;
    int id=0;
    char buffer[sizeof(ConnectInit)];

    sock = (int) arg;

    //handle if there are too many users    
    if(numUsers >= maxUsers)
    {
        recv(sock, buffer, sizeof(ConnectInit), 0);
        ConnectInit * cI = &buffer;
        char * name = cI->userName;
        printf("WARNING you have too many users UserName:%s  Current Users: %i\n",name,numUsers);
        ConnectACK ack;
        ack.id = 0;
        ack.status = 1;
        send(sock, &ack, sizeof(ConnectACK), 0);

        close(sock);
        pthread_exit(0); 
    }

    //if(username Already Exists)
    //{
    //}
   
    sem_wait(&users); 
    numUsers++;
    totalUsers++;
    id = totalUsers;
    sem_post(&users);


    //the ConnectINIT part
    recv(sock, buffer, sizeof(ConnectInit), 0);
    ConnectInit * cI = &buffer;
    char * name = cI->userName;
    printf("UserID : %i  Username:%s  Current Users: %i\n", id, name, numUsers);
        
    ConnectACK ack;
    ack.id = totalUsers;
    ack.status = 0;

    send(sock, &ack, sizeof(ConnectACK), 0);

    sleep(35);

    //quiting code
    close(sock);
    sem_wait(&users);
    numUsers--;
    sem_post(&users);

    printf("User id: %i disconnected, Remaining Users: %i\n", id, numUsers);
}
