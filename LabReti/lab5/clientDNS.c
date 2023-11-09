#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include "../lab1/wrapper.h"

int main(int argc, char **argv) {
    int sockfd, n;
    char recvline[1025];
    struct hostent* hname;
    struct sockaddr_in servaddr;

    // Check the correct number of command-line arguments
    if (argc != 2) {
        fprintf(stderr, "usage: %s <IPaddress>\n", argv[0]);
        exit(1);
    }

    // Resolve the hostname provided by the user
    hname = gethostbyname(argv[1]);
    if (!hname) {
        perror("gethostbyname");
        exit(1);
    }

    // Create a socket
    sockfd = Socket(AF_INET, SOCK_STREAM, 0);

    // Configure the sockaddr_in structure for the connection
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(1025);
    servaddr.sin_addr = *((struct in_addr *)hname->h_addr_list[0]);

    // Connect to the server
    Connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

    // Print the connected IP address
    // char ipAddress[INET_ADDRSTRLEN];
    // if (inet_ntop(AF_INET, &(servaddr.sin_addr), ipAddress, sizeof(ipAddress)) == NULL) {
    //     perror("inet_ntop");
    //     exit(1);
    // }
    // printf("Connected to: %s\n", ipAddress);

    // Read data from the socket
    n = FullRead(sockfd, recvline, 1024);
    if (n < 0) {
        perror("read error");
        exit(1);
    }

    // Print the received data to stdout
    if (fputs(recvline, stdout) == EOF) {
        fprintf(stderr, "fputs error\n");
        exit(1);
    }

    // Close the socket after reading the data
    close(sockfd);

    // Exit the program
    exit(0);
}
