#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include "../lab1/wrapper.h"

int main(int argc, char **argv)
{
    int listenfd, connfd;
    struct sockaddr_in servaddr;
    int charRead = 0;
    char buff[1025];
    char buff2[1025];
    listenfd = Socket(AF_INET, SOCK_STREAM, 0);

    // Initialize server address structure
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(1025);

    // Bind the socket to the server address
    Bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

    // Listen for incoming connections
    Listen(listenfd, 1024);

    for (;;)
    {
        // Accept a connection from the client
        connfd = Accept(listenfd, (struct sockaddr *)NULL, NULL);
       
        // Read the actual string from the client
        FullRead(connfd, buff, 1024);

        // Calculate the number of characters read
        charRead = strlen(buff);

        // Prepare the response string
        snprintf(buff2, sizeof(buff2), "Reading %d char from client\n", charRead);

        // Send the response back to the client
        FullWrite(connfd, buff2, sizeof(buff2));
       
        // Close the connection with the client
        close(connfd);
    }
}
