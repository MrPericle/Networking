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
    socklen_t len;
    struct sockaddr_in clientaddr;
    char ipAddress[INET_ADDRSTRLEN];

    listenfd = Socket(AF_INET, SOCK_STREAM, 0);

    // Inizializzazione diretta della struttura servaddr
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(1025);

    Bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

    Listen(listenfd, 1024);

    for (;;)
    {
        len = sizeof(clientaddr);
        connfd = Accept(listenfd, (struct sockaddr *)&clientaddr, &len);

        if (inet_ntop(AF_INET, &(clientaddr.sin_addr), ipAddress, sizeof(ipAddress)) == NULL)
        {
            perror("inet_ntop");
            exit(1);
        }

        ticks = time(NULL);
        snprintf(buff, sizeof(buff), "Serving client with IP: %s\n%.24s\r\n",ipAddress, ctime(&ticks));
        FullWrite(connfd, buff, strlen(buff)); // Use strlen to get the actual length of the string

        close(connfd);
    }
}
