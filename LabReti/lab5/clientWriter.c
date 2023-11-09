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
    struct sockaddr_in servaddr;

    if (argc != 3) {
        fprintf(stderr, "usage: %s <IPaddress>\n", argv[0]);
        exit(1);
    }

    sockfd = Socket(AF_INET, SOCK_STREAM, 0);
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(1025);

    if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0) {
        fprintf(stderr, "inet_pton error for %s\n", argv[1]);
        exit(1);
    }

    Connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
    printf("Connection success\n");

    // Calcola la lunghezza della stringa da inviare
    size_t str_len = strlen(argv[2]);

    // Invia la lunghezza della stringa
    FullWrite(sockfd, &str_len, sizeof(size_t));

    // Invia la stringa stessa
    FullWrite(sockfd, argv[2], str_len);

    n = FullRead(sockfd, recvline, 1024);
    if (n < 0) {
        perror("read error");
        exit(1);
    }

    // Stampa i dati ricevuti su stdout
    if (fputs(recvline, stdout) == EOF) {
        fprintf(stderr, "fputs error\n");
        exit(1);
    }

    exit(0);
}
