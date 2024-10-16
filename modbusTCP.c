#include "modbusTCP.h"

#define PORT 22222
#define IP "127.0.0.1"
#define BUF_LEN 1024

uint16_t TI = 0;


int send_modbus_request (char* server_addr, int port, uint8_t* APDU, int APDU_len, uint8_t* APDU_R) {
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
        perror("socket");
        return 1;
    }

    sad_loc.sin_family = PF_INET;
    sad_loc.sin_port = htons(PORT);
    inet_aton(IP, &sad_loc.sin_addr);

    conn = connect(sock, (struct sockaddr *)&sad_loc, sizeof(sad_loc));
    if (conn < 0) {
        perror("connect");
        return 1;
    }

    // write (fd, PDU, PDUlen) // sends Modbus TCP PDU
    write(sock, MBAP, 7);
    write(sock, APDU, APDU_len);

    // read (fd, PDU_R, PDU_Rlen) // response o timeout
    read(sock, MBAP_R, 7);
    read(sock, APDU_R, APDU_R[5] - 1);

    // if response, remove MBAP, PDU_R → APDU_R
    // closes TCP client socket with server (*)
    close(sock);
    
    // returns: APDU_R and 0 – ok, <0 – error (timeout)
    APDU_R[0] = 0x00;

    return 0; 
}