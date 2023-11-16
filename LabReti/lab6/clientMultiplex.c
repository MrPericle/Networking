#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <arpa/inet.h>
#include <string.h>
#include "../lab1/wrapper.h"

#define MAX_BUFFER_SIZE 1024

#define max(x, y) ({typeof (x) x_ = (x); typeof (y) y_ = (y); \
    x_ > y_ ? x_ : y_;})

int main(int argc, char **argv) {
    int sockfd, n, maxd;
    char recvline[MAX_BUFFER_SIZE];
    char printBuffer[MAX_BUFFER_SIZE];
    struct sockaddr_in servaddr = {0};
    fd_set set;

    // Check the correct number of command-line arguments
    if (argc != 2) {
        fprintf(stderr, "usage: %s <IPaddress>\n", argv[0]);
        exit(1);
    }

    // Create a socket
    sockfd = Socket(AF_INET, SOCK_STREAM, 0);

    // Configure the server address structure
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(1025);

    // Convert the IP address from string to binary form
    if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0) {
        fprintf(stderr, "inet_pton error for %s\n", argv[1]);
        exit(1);
    }

    // Establish a connection to the server
    Connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

    while (1) {
        FD_ZERO(&set);
        FD_SET(sockfd, &set);
        FD_SET(STDIN_FILENO, &set);

        maxd = max(STDIN_FILENO, sockfd) + 1;

        if (select(maxd, &set, NULL, NULL, NULL) < 0)
            exit(1);

        if (FD_ISSET(STDIN_FILENO, &set)) {
            if (fgets(printBuffer, MAX_BUFFER_SIZE, stdin) == NULL) 
                exit(EXIT_FAILURE);
            printBuffer[strlen(printBuffer)-1] = '\0';
            FullWrite(sockfd, printBuffer, sizeof(printBuffer));
            fflush(stdout);
            printf("Read from STDIN: %s\n", printBuffer);
        }

        if (FD_ISSET(sockfd, &set)) {
            FullRead(sockfd, recvline, MAX_BUFFER_SIZE);
            if (strlen(recvline) == 0) {
                printf("Server closed the connection. Exiting...\n");
                break;
            }
            printf("Read from socket: %s\n", recvline);
        }
    }
    close(sockfd);

    return 0;
}
