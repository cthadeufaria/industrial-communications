#include <stdio.h>
#include <stdint.h>
#include "modbusAP.h"



int write_multiple_registers (char* server_addr, int port, uint16_t st_r, uint16_t n_r, uint16_t* val) {
    uint8_t apdu[MAX_APDU], APDU_R[5];
    int apdu_len;

    // check consistency of parameters
    if (server_addr == NULL || port <= 0 || val == NULL || n_r < 1 || n_r > 123) {
        fprintf(stderr, "Invalid parameters\n");
        return -1;
    }

    // assembles APDU (MODBUS PDU)
    apdu[0] = 0x10; // Function code for Write Multiple Registers
    apdu[1] = (uint8_t) (st_r >> 8); // Starting address high byte
    apdu[2] = (uint8_t) st_r; // Starting address low byte
    apdu[3] = (uint8_t) (n_r >> 8); // Quantity of registers high byte
    apdu[4] = (uint8_t) n_r; // Quantity of registers low byte
    apdu[5] = (uint8_t) n_r * 2; // Byte count

    for (int i = 0; i < n_r; i++) {
        apdu[6 + 2 * i] = (uint8_t) (val[i] >> 8); // Register value high byte
        apdu[7 + 2 * i] = (uint8_t) val[i]; // Register value low byte
    }

    // debug: print apdu
    printf("APDU: ");
    for (int i = 0; i < 6 + 2 * n_r; i++) {
        printf("%.2x ", apdu[i]);
    }
    printf("\n");

    // Send_Modbus_request (server_add,port,APDU,APDUlen,APDU_R)
    int result = Send_Modbus_request(server_addr, port, apdu, apdu_len, APDU_R);
    if (result < 0) {
        fprintf(stderr, "Failed to send Modbus request\n");
        return -2;
    }

    // Process the response in APDU_R if needed
    // Check if the response function code matches the request function code
    if (APDU_R[0] & 0x80) {
        fprintf(stderr, "Modbus error\n");
        return -APDU_R[1];
    }

    // debug: print APDU_R
    printf("APDU_R: ");
    for (int i = 0; i < 5; i++) {
        printf("%.2x ", APDU_R[i]);
    }
    printf("\n");

    // // Check if the response contains the correct starting address and quantity of registers
    // uint16_t resp_st_r = (APDU_R[1] << 8) | APDU_R[2];
    // uint16_t resp_n_r = (APDU_R[3] << 8) | APDU_R[4];

    // if (resp_st_r != st_r || resp_n_r != n_r) {
    //     fprintf(stderr, "Mismatch in response starting address or quantity of registers\n");
    //     return -1;
    // }

    // checks the reponse (APDU_R or error_code)
    // returns: number of written registers – ok, <0 – error
    return n_r;
}



void read_holding_registers (server_addr, port, st_r, n_r, val) {
    // check consistency of parameters
    // assembles APDU (MODBUS PDU)
    // Send_Modbus_request (server_add,port,APDU,APDUlen,APDU_R)
    // checks the reponse (APDU_R or error_code)
    // returns: number of read registers – ok, <0 – error
}