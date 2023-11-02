#include<sys/types.h> /* predefined types */
#include<unistd.h> /* include unix standard library */
#include<arpa/inet.h> /* IP addresses conversion utililites */
#include<sys/socket.h> /* socket library */
#include<stdio.h> /* include standard I/O library */
#include <stdlib.h>
#include <string.h>
#include<time.h>
#include <stdatomic.h>
#include "../lab1/wrapper.h"

atomic_int clientNum = 0;

void handler(int sigum){
    if(sigum == SIGCHLD){
        clientNum++;
        //printf("Signal %d cought, clientNum increased %d",SIGCHLD,clientNum);
    }
}

int main (int argc , char *argv[])
{
    int list_fd,conn_fd;
    int i;
    struct sockaddr_in serv_add,client;
    char buffer [1024];
    socklen_t len;
    time_t timeval;
    pid_t pid;
    int logging =1;
    /*COMPLETE HERE*/
    /* write daytime to client */
    signal(SIGCHLD,handler);
    list_fd = Socket(AF_INET,SOCK_STREAM,0);
    serv_add.sin_family = AF_INET;
    serv_add.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_add.sin_port = htons(1024);

    Bind(list_fd, (struct sockaddr *) &serv_add, sizeof(serv_add));

    Listen(list_fd, 1024);

    while(1)
    {
        len = sizeof ( client );
        /*accept*/
        /*COMPLETE HERE*/
        /* fork to handle connection */
        conn_fd = Accept(list_fd, (struct sockaddr *) NULL, NULL); 
        if((pid = fork())<0)
            {
            perror (" fork error ");
            exit ( -1);
            }
        if(pid==0)
            { /* child */
            close ( list_fd );
            timeval = time ( NULL );
            snprintf(buffer,sizeof(buffer),"Sei il client numero %d servito. %.24s\r\n",clientNum,ctime(& timeval));
            /*write*/
            /*COMPLETE HERE*/
            FullWrite(conn_fd,buffer,sizeof(buffer));
            if(logging)
                {
                inet_ntop(AF_INET,&client.sin_addr,buffer,sizeof(buffer));
                printf("Request from host %s, port %d\n",buffer,ntohs(client.sin_port));
                }
            /*close*/
            /*COMPLETE HERE*/
            close(conn_fd);
            exit (0);
            }
        else 
            { /* parent */
            close ( conn_fd );
            }
    }
    /* normal exit , never reached */
    exit (0);
}

