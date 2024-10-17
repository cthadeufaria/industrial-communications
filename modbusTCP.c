#include "modbusTCP.h"
#include <errno.h>

#define BUF_LEN 1024
#define UNIT_ID 1
#define DEBUG 0

uint16_t TI = 0;


int send_modbus_request (char *server_addr, int port, uint8_t* APDU, int APDU_len, uint8_t *APDU_R) {
    uint8_t MBAP[7], MBAP_R[7]={0, 0, 0, 0, 0, 0, 0};
    int sock, len, conn;
    socklen_t addr_len = sizeof(server_addr);
    struct sockaddr_in sad_loc;

    // generates TI (trans.ID →sequence number)
    TI++;
    MBAP[0] = (uint8_t) (TI >> 8);
    MBAP[1] = (uint8_t) TI;

    // assembles PDU = APDU(SDU) + MBAP
    // Protocol identifier
    uint16_t PI = 0;
    MBAP[2] = (uint8_t) (PI >> 8);
    MBAP[3] = (uint8_t) PI;

    // Length 2 bytes
    MBAP[4] = 0;
    MBAP[5] = (uint8_t) (APDU_len + 1);

    // Unit identifier
    MBAP[6] = UNIT_ID;

    // opens TCP client socket and connects to server (*)
    sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        fprintf(stderr, "Error: Failed to create socket for Modbus TCP connection.\n");
        fprintf(stderr, "System Error [%d]: %s\n", errno, strerror(errno));
        perror("socket");
        return -1;
    }

    sad_loc.sin_family = PF_INET;
    sad_loc.sin_port = htons(port);
    inet_aton(server_addr, &sad_loc.sin_addr);
    if (inet_aton(server_addr, &sad_loc.sin_addr) == 0) {
        fprintf(stderr, "Invalid IP address: %s\n", server_addr);
        return -1;
    }
    
    conn = connect(sock, (struct sockaddr *)&sad_loc, sizeof(sad_loc));
    if (conn < 0) {
        fprintf(stderr, "Error: Failed to connect to server at %s:%d. Reason: %s\n", server_addr, port, strerror(errno));
        return -1;
    }

    // write (fd, PDU, PDUlen) // sends Modbus TCP PDU
    int bytes_written = write(sock, MBAP, 7);
    if (bytes_written != 7) {
        fprintf(stderr, "Error: Failed to send MBAP header. Expected to write 7 bytes, but wrote %d bytes. Reason: %s\n", bytes_written, strerror(errno));
        close(sock);
        return -1;
    }

    bytes_written = write(sock, APDU, APDU_len);
    if (bytes_written != APDU_len) {
        fprintf(stderr, "Error: Failed to send APDU. Expected to write %d bytes, but wrote %d bytes. Reason: %s\n", APDU_len, bytes_written, strerror(errno));
        close(sock);
        return -1;
    }
    
    // Read the MBAP response header
    int bytes_read = read(sock, MBAP_R, 7);
    if (bytes_read <= 0) {
        if (bytes_read == 0) {
            fprintf(stderr, "Error: Connection closed by the server (bytes_read = %d)\n", bytes_read);
        } else {
            perror("Error reading MBAP header");
        }
        close(sock);
        return -1;
    } else if (bytes_read != 7) {
        fprintf(stderr, "Error: Incomplete MBAP header (bytes_read = %d)\n", bytes_read);
        close(sock);
        return -1;
    }

    // Get the length of the APDU response from MBAP_R[4] and MBAP_R[5]
    int apdu_response_length = (MBAP_R[4] << 8) | MBAP_R[5];
    int bytes_to_read = apdu_response_length - 1; // Subtract the Unit ID byte

    // Read the APDU response
    if (bytes_to_read > 0) {
        bytes_read = read(sock, APDU_R, bytes_to_read);
        if (bytes_read != bytes_to_read) {
            fprintf(stderr, "Error: Failed to read APDU response (bytes_read = %d)\n", bytes_read);
            close(sock);
            return -1;
        }
    } else {
        fprintf(stderr, "Error: Invalid response length in MBAP header\n");
        close(sock);
        return -1;
    }

    // closes TCP client socket with server (*)
    close(sock);

    // DEBUG prints
    if (DEBUG) {
        printf("APDU_len: %d\n", APDU_len);
        printf("APDU: ");
        for (int i = 0; i < APDU_len; i++) {
            printf("%.2x ", APDU[i]);
        }
        printf("\n");
        printf("MBAP: ");
        for (int i = 0; i < 7; i++) {
            printf("%.2x ", MBAP[i]);
        }
        printf("\n");
        printf("MBAP_R: ");
        for (int i = 0; i < 7; i++) {
            printf("%.2x ", MBAP_R[i]);
        }
        printf("\n");
        printf("APDU_R: ");
        for (int i = 0; i < bytes_to_read; i++) {
            printf("%.2x ", APDU_R[i]);
        }
        printf("\n");
    }

    return 0; 
}