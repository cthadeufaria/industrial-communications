#include "modbusTCP.h"

#define BUF_LEN 1024

uint16_t TI = 0;


int send_modbus_request (char *server_addr, int port, uint8_t* APDU, int APDU_len, uint8_t *APDU_R) {
    uint8_t MBAP[7], MBAP_R[7]={0, 0, 0, 0, 0, 0, 0};
    int sock, len, conn;
    socklen_t addr_len = sizeof(server_addr);
    struct sockaddr_in sad_loc;

    printf("APDU_len: %d\n", APDU_len);

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
        perror("socket");
        return 1;
    }

    sad_loc.sin_family = PF_INET;
    sad_loc.sin_port = htons(port);
    // sad_loc.sin_addr.s_addr = inet_addr(server_addr);
    inet_aton(server_addr, &sad_loc.sin_addr);
    if (inet_aton(server_addr, &sad_loc.sin_addr) == 0) {
        fprintf(stderr, "Invalid IP address: %s\n", server_addr);
        return 1;
    }
    printf("Connecting to IP %s on port %d (network byte order: %d)\n", server_addr, port, htons(port));
    
    conn = connect(sock, (struct sockaddr *)&sad_loc, sizeof(sad_loc));
    if (conn < 0) {
        perror("Error in connect");  // Prints a descriptive error message
        return -1;
    } else {
        printf("Successfully connected!\n");
    }

    // write (fd, PDU, PDUlen) // sends Modbus TCP PDU
    int bytes_written = write(sock, MBAP, 7);
    if (bytes_written != 7) {
        fprintf(stderr, "Error: Failed to send MBAP (bytes_written = %d)\n", bytes_written);
        close(sock);
        return -1;
    }

    printf("APDU: ");
    for (int i = 0; i < APDU_len; i++) {
        printf("%.2x ", APDU[i]);
    }
    printf("\n");

    bytes_written = write(sock, APDU, APDU_len);
    if (bytes_written != APDU_len) {
        perror("Error: Failed to send APDU");
        close(sock);
        return -1;
    } else {
        printf("APDU successfully sent (%d bytes)\n", bytes_written);
    }

    printf("MBAP: ");
    for (int i = 0; i < 7; i++) {
        printf("%.2x ", MBAP[i]);
    }
    printf("\n");
    
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

    printf("MBAP_R: ");
    for (int i = 0; i < 7; i++) {
        printf("%.2x ", MBAP_R[i]);
    }
    printf("\n");


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

        printf("APDU_R: ");
        for (int i = 0; i < bytes_to_read; i++) {
            printf("%.2x ", APDU_R[i]);
        }
        printf("\n");
    } else {
        fprintf(stderr, "Error: Invalid response length in MBAP header\n");
        close(sock);
        return -1;
    }

    // if response, remove MBAP, PDU_R → APDU_R
    // closes TCP client socket with server (*)
    close(sock);
    
    // returns: APDU_R and 0 – ok, <0 – error (timeout)
    // APDU_R[0] = 0x00;

    return 0; 
}