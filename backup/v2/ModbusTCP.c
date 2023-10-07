#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "ModbusTCP.h"
#include "ModbusAP.h"

int send_modbus_request(const char *server_address, int port, const uint8_t *apdu, int apdu_len, uint8_t *apdu_response) {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("Socket creation failed");
        return -1;
    }

    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    if (inet_pton(AF_INET, server_address, &server.sin_addr) <= 0) {
        perror("Invalid server address");
        close(sockfd);
        return -1;
    }

    if (connect(sockfd, (struct sockaddr *)&server, sizeof(server)) == -1) {
        perror("Connection failed");
        close(sockfd);
        return -1;
    }

    // Send the Modbus TCP PDU
    if (write(sockfd, apdu, apdu_len) == -1) {
        perror("Failed to send Modbus request");
        close(sockfd);
        return -1;
    }

    // Receive the response
    int bytes_received = read(sockfd, apdu_response, apdu_len);
    if (bytes_received == -1) {
        perror("Failed to receive Modbus response");
        close(sockfd);
        return -1;
    }

    close(sockfd);
    return bytes_received;
}
