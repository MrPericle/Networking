#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include "../lab1/wrapper.h"

int main(int argc, char **argv) {
    int sockfd, n;
    char recvline[1025];
    char printBuffer[1025];
    struct sockaddr_in servaddr;

    // Check the correct number of command-line arguments
    if (argc != 3) {
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

    // Calculate the length of the string to send
    size_t str_len = strlen(argv[2]);

    // Send the actual string to the server
    snprintf(printBuffer,sizeof(printBuffer),"%s",argv[2]);
    FullWrite(sockfd, printBuffer, sizeof(printBuffer));
   
    // Read the response from the server
    FullRead(sockfd, recvline, 1024);

    // Print the received data to stdout
    if (fputs(recvline, stdout) == EOF) {
        fprintf(stderr, "fputs error\n");
        exit(1);
    }
    
    // Close the socket and exit the program
    close(sockfd);
    exit(0);
}
