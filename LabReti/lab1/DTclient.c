#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "wrapper.h"

int main(int argc, char **argv) {
    int sockfd, n;
    char recvline[1025] ;
    struct sockaddr_in servaddr; 
    if (argc != 2) {
        fprintf(stderr,"usage: %s <IPaddress>\n",argv[0]);
        exit (1); 
    }
    sockfd = Socket(AF_INET,SOCK_STREAM,0);
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(13);
    if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0) {
        fprintf(stderr,"inet_pton error for %s\n", argv[1]); exit (1);
    }
    Connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr));

    FullRead(sockfd,recvline, 1024);
    
    if (fputs(recvline, stdout) == EOF) {
    fprintf(stderr,"fputs error\n");
    exit(1); 
    }

    if (n < 0) {
        fprintf(stderr,"read error\n");
        exit(1); 
    }
    exit(0); 
}