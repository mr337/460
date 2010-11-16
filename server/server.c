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

//prototypes
void* thread_proc(void *arg);
int setUserName(char * name);
int checkDupUserName(char * name);

int maxUsers;
int numUsers;
int totalUsers;
char **userNames;
int * userStatus;
sem_t lusers;

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
    sem_init(&lusers,0,1);
    numUsers = 0;
    totalUsers = 0;
    userStatus = malloc(maxUsers * sizeof(int));

    //init of arruoay of strings
    userNames = malloc(maxUsers * sizeof(char *));
    int i = 0;
    for(i=0; i< maxUsers; i++)
    {
        userNames[i] = malloc(UNAMELENGTH * sizeof(char));
        strcpy(userNames[i],"\0");
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

        //test code
        for(i=0; i<maxUsers; i++)
        {
            printf("Userid: %i    Name: %s\n",i, userNames[i]);
        }
    }

    return(EXIT_SUCCESS);
}

void* thread_proc(void *arg)
{
    int sock;
    int id=0;
    char buffer[sizeof(ConnectInit)];

    sock = (int) arg;

    //the ConnectINIT part
    recv(sock, buffer, sizeof(ConnectInit), 0);
    ConnectInit * cI = &buffer;
    char * name = cI->userName;


    //form ACK response
    ConnectACK ack;
    
    //handle bad status    
    if(numUsers >= maxUsers)
    {
        printf("WARNING you have too many users UserName:%s  Current Users: %i\n",name,numUsers);
        ack.id = 0;
        ack.status = 1;
    }
    else if(checkDupUserName(name))
    {
        printf("ERROR: duplicate username found\n");
        ack.id = 0;
        ack.status = 2;
    }
    else if(strlen(name) == 0 && name[0] != ' ')
    {
        printf("ERROR: username is blank or length of zero\n");
        ack.id=0;
        ack.status=4;
    }
    else
    {
        sem_wait(&lusers); 
        numUsers++;
        totalUsers++;
        id = setUserName(name);
        if(id == -1)
        {
            printf("Error getting an ID for user\n");
            ack.status = 3;
        }
        sem_post(&lusers);

        ack.id = id;
        //ack.status already set to 0 unless failed to get unique user id
    }

    send(sock, &ack, sizeof(ConnectACK), 0);

    if(ack.status != 0)
    { //if error occured will free resources asap
        close(sock);
        pthread_exit(0); 
    }
    

    printf("UserID : %i  Username:%s  Current Users: %i\n", id, name, numUsers);


    sleep(3);

    //quiting code
    close(sock);
    sem_wait(&lusers);
    numUsers--;
    strcpy(userNames[id], "\0");
    sem_post(&lusers);

    printf("User id: %i disconnected, Remaining Users: %i\n", id, numUsers);
}

int setUserName(char * name)
{
    int i;
    for(i=0;i<maxUsers;i++)
    {
        if(!strcmp(userNames[i], "\0"))
        {
            strcpy(userNames[i],name);
            return i;
        }
    }

    return -1; //means too many users (something majorly wrong) or username exists
}

int checkDupUserName(char * name)
{
    int i;
    for(i=0; i<maxUsers;i++)
    {
        if(!strcmp(userNames[i], name))
        {
            return 1;
        }
    }

    return 0; //no match 
}
