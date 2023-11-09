#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <time.h>
#include <netdb.h>
#include "../lab1/wrapper.h"

int main(int argc, char **argv)
{
    int listenfd, connfd;
    struct sockaddr_in servaddr;
    char buff[4096];
    time_t ticks;

    listenfd = Socket(AF_INET,SOCK_STREAM,0);

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(13);

    if ( bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0 )
    {
        perror("bind");
        exit(1);
    }

    Listen(listenfd, 1024);

    for ( ; ; ) {
        connfd = Accept(listenfd, (struct sockaddr *) NULL, NULL); 
        ticks = time(NULL);
        snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
        FullWrite(connfd,buff,sizeof(buff));

        close(connfd);
    }
}