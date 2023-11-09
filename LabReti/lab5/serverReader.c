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
    char buff[4096];
    char buff2[4096];
    listenfd = Socket(AF_INET, SOCK_STREAM, 0);

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(1025);

    Bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

    Listen(listenfd, 1024);

    for (;;)
    {
        connfd = Accept(listenfd, (struct sockaddr *)NULL, NULL);

        size_t str_len;
        FullRead(connfd, &str_len, sizeof(size_t));

        // Leggi la stringa effettiva
        FullRead(connfd, buff, str_len);
        buff[str_len] = '\0';  

        charRead = strlen(buff);
        snprintf(buff2, sizeof(buff2), "Reading %d char from client\n", charRead);
        FullWrite(connfd, buff2, strlen(buff2)); // Use strlen to get the actual length of the string

        close(connfd);
    }
}
