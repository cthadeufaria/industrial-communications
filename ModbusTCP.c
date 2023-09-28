#include "ModbusTCP.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// Function to establish a Modbus TCP connection
int modbus_tcp_connect(const char *server_ip, int server_port, int *sockfd) {
    struct sockaddr_in serv_addr;

    // Create a socket
    *sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (*sockfd < 0) {
        perror("Socket creation failed");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(server_port);
    serv_addr.sin_addr.s_addr = inet_addr(server_ip);

    // Connect to the server
    if (connect(*sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection failed");
        return -1;
    }

    return 0; // Connection successful
}

// Function to close a Modbus TCP connection
void modbus_tcp_close(int sockfd) {
    close(sockfd);
}