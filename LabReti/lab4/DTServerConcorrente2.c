#include <sys/types.h>   // Predefined types
#include <unistd.h>      // Include Unix standard library
#include <arpa.inet.h>   // IP address conversion utilities
#include <sys.socket.h>  // Socket library
#include <stdio.h>       // Include standard I/O library
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <signal.h>     // Include for signal handling
#include "../lab1/wrapper.h"

int clientNum = 0;

void handler(int signum) {
    if (signum == SIGCHLD) {
        //When a child process ends, increment global variable
        clientNum++;
    }
}

int main(int argc, char *argv[]) {
    int list_fd, conn_fd;
    int i;
    struct sockaddr_in serv_add, client;
    char buffer[1024];
    socklen_t len;
    time_t timeval;
    pid_t pid;
    int logging = 1;

    // Set up a signal handler for SIGCHLD
    signal(SIGCHLD, handler);

    // Create a socket
    list_fd = Socket(AF_INET, SOCK_STREAM, 0);
    serv_add.sin_family = AF_INET;
    serv_add.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_add.sin_port = htons(1024);

    // Bind the socket to the address
    Bind(list_fd, (struct sockaddr *) &serv_add, sizeof(serv_add));

    // Listen for incoming connections
    Listen(list_fd, 1024);

    while (1) {
        len = sizeof(client);

        // Accept an incoming connection
        conn_fd = Accept(list_fd, (struct sockaddr *) NULL, NULL);

        if ((pid = fork()) < 0) {
            perror("Fork error");
            exit(-1);
        }

        if (pid == 0) {  // Child process
            close(list_fd);
            timeval = time(NULL);
            snprintf(buffer, sizeof(buffer), "You are client number %d served. %.24s\r\n", clientNum, ctime(&timeval));

            // Write to the client
            FullWrite(conn_fd, buffer, sizeof(buffer));

            if (logging) {
                char ipBuffer[INET_ADDRSTRLEN];
                inet_ntop(AF_INET, &client.sin_addr, ipBuffer, sizeof(ipBuffer));
                printf("Request from host %s, port %d\n", ipBuffer, ntohs(client.sin_port));
            }

            // Close the connection
            close(conn_fd);
            exit(0);
        } else {  // Parent process
            close(conn_fd);
        }
    }

    // Normal exit, never reached
    exit(0);
}
