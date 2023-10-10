#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "ModbusTCP.h"

void send_modbus_request(const char* ip, int port, const char* APDU, int APDUlen, char* APDU_R) {
    int protocol_identifier = 0x0000;
    int unitID = 0x11;
    int transaction_id = 0x0001;

    // Assemble PDU = Mbap header + APDU.
    char mbap[7];
    mbap[0] = (transaction_id >> 8) & 0xFF;
    mbap[1] = transaction_id & 0xFF;
    mbap[2] = (protocol_identifier >> 8) & 0xFF;
    mbap[3] = protocol_identifier & 0xFF;
    mbap[4] = ((APDUlen + 1) >> 8) & 0xFF;
    mbap[5] = (APDUlen + 1) & 0xFF;
    mbap[6] = unitID;

    char PDU[APDUlen + 7];
    memcpy(PDU, mbap, 7);
    memcpy(PDU + 7, APDU, APDUlen);
    printf(PDU);

    // Open TCP client socket and connect to server.
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    inet_pton(AF_INET, ip, &(server_addr.sin_addr));
    connect(fd, (struct sockaddr*)&server_addr, sizeof(server_addr));

    // Send Modbus TCP PDU.
    send(fd, PDU, APDUlen + 7, 0);

    printf(PDU);

    // Get server response.
    recv(fd, APDU_R, APDUlen + 8, 0);

    // Remove MBAP and get APDU response.
    memcpy(APDU_R, APDU_R + 7, APDUlen);

    // Close TCP client socket with server.
    close(fd);
}
