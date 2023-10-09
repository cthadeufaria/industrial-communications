#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define UNIT_ID 0x11

void send_modbus_request(const char *ip, int port, uint8_t *APDU, int APDUlen) {
    int fd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(ip);

    connect(fd, (struct sockaddr*)&server_addr, sizeof(server_addr));

    // Generate transaction ID.
    uint16_t transaction_id = (uint16_t)rand();

    // Assemble Mbap header.
    uint8_t mbap[7];
    mbap[0] = (uint8_t)(transaction_id >> 8);
    mbap[1] = (uint8_t)(transaction_id & 0xFF);
    mbap[2] = 0x00;
    mbap[3] = 0x00;
    mbap[4] = (uint8_t)((APDUlen + 1) >> 8);
    mbap[5] = (uint8_t)((APDUlen + 1) & 0xFF);
    mbap[6] = UNIT_ID;

    // Send Mbap header + APDU.
    send(fd, (char *)mbap, sizeof(mbap), 0);
    send(fd, (char *)APDU, APDUlen, 0);

    // Get server response.
    uint8_t PDU_R[APDUlen + 8];
    int bytes_received = recv(fd, (char *)PDU_R, sizeof(PDU_R), 0);

    // Remove Mbap header and get APDU response.
    if (bytes_received > 7) {
        uint8_t *APDU_R = PDU_R + 7;
        printf("PDU response: ");
        for (int i = 0; i < bytes_received - 7; i++) {
            printf("%02X ", APDU_R[i]);
        }
        printf("\n");
        printf("APDU response: ");
        for (int i = 0; i < bytes_received - 7; i++) {
            printf("%02X ", APDU_R[i]);
        }
        printf("\n");
    }

    // Close TCP client socket with the server.
    close(fd);
}

// int main() {
//     const char *ip = "127.0.0.1"; // Replace with your server's IP address
//     int port = 502;               // Replace with your server's port
//     uint8_t APDU[] = {0x01, 0x02, 0x00, 0x00, 0x00, 0x06, 0x01, 0x03, 0x00, 0x00, 0x00, 0x02}; // Replace with your APDU
//     int APDUlen = sizeof(APDU);

//     send_modbus_request(ip, port, APDU, APDUlen);

//     return 0;
// }
