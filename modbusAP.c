#include <stdio.h>
#include <stdint.h>
#include "modbusTCP.h"

#define MAX_APDU 253



int write_multiple_registers (char *server_addr, int port, uint16_t st_r, uint16_t n_r, uint16_t *val) {
    uint8_t apdu[MAX_APDU], APDU_R[5];
    int apdu_len;

    // check consistency of parameters
    if (server_addr == NULL || port <= 0 || val == NULL || n_r < 1 || n_r > 123) {
        fprintf(stderr, "Invalid parameters\n");
        return -1;
    }

    // assembles APDU (MODBUS PDU)
    apdu[0] = 0x10; // Function code for Write Multiple Registers
    apdu[1] = (uint8_t) ((st_r -1)>> 8); // Starting address high byte
    apdu[2] = (uint8_t) (st_r -1); // Starting address low byte
    apdu[3] = (uint8_t) (n_r >> 8); // Quantity of registers high byte
    apdu[4] = (uint8_t) n_r; // Quantity of registers low byte
    apdu[5] = (uint8_t) n_r * 2; // Byte count

    for (int i = 0; i < n_r; i++) {
        apdu[6 + 2 * i] = (uint8_t) (val[i] >> 8); // Register value high byte
        apdu[7 + 2 * i] = (uint8_t) val[i]; // Register value low byte
    }

    apdu_len = 6 + 2 * n_r;
    
    // Send_Modbus_request (server_add,port,APDU,APDUlen,APDU_R)
    int result = send_modbus_request(server_addr, port, apdu, apdu_len, APDU_R);
    if (result < 0) {
        fprintf(stderr, "Failed to send Modbus request\n");
        return -1;
    }

    // Check if the response function code matches the request function code
    if (APDU_R[0] & 0x80) {
        printf("Modbus error: Exception Code %d\n", APDU_R[1]);
        return -1;
    }

    return 0;
}


int read_holding_registers (char *server_addr, int port, uint16_t st_r, uint16_t n_r, uint16_t *val) {
    uint8_t apdu[MAX_APDU], APDU_R[MAX_APDU];
    int apdu_len;

    // check consistency of parameters
    if (server_addr == NULL || port <= 0 || val == NULL || n_r < 1 || n_r > 125) {
        fprintf(stderr, "Invalid parameters\n");
        return -1;
    }

    // assembles APDU (MODBUS PDU)
    apdu[0] = 0x03; // Function code for Read Holding Registers
    apdu[1] = (uint8_t) ((st_r -1) >> 8); // Starting address high byte
    apdu[2] = (uint8_t) (st_r -1); // Starting address low byte
    apdu[3] = (uint8_t) (n_r >> 8); // Quantity of registers high byte
    apdu[4] = (uint8_t) n_r; // Quantity of registers low byte

    apdu_len = 5; // APDU length for Read Holding Registers is always 5 bytes

    // Send_Modbus_request (server_add,port,APDU,APDUlen,APDU_R)
    int result = send_modbus_request(server_addr, port, apdu, apdu_len, APDU_R);
    if (result < 0) {
        fprintf(stderr, "Failed to send Modbus request\n");
        return -1;
    }

    // Check if the response function code matches the request function code
    if (APDU_R[0] & 0x80) {
        printf("Modbus error: Exception Code %d\n", APDU_R[1]);
        return -1;
    }

    // The expected response starts at APDU_R[1] (Byte count), followed by register values
    int byte_count = APDU_R[1];

    // Extract register values from response
    for (int i = 0; i < n_r; i++) {
        val[i] = (APDU_R[2 + i * 2] << 8) | APDU_R[3 + i * 2]; // Combine high and low bytes
    }

    return 0;
}