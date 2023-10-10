#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "ModbusTCP.h"

int write_multiple_regs(const char* server_address, int port, const char* start_register, const char* num_registers, const char* values) {
    // Convert start_register and num_registers from hexadecimal strings to integers
    int start_reg = strtol(start_register, NULL, 16);
    int num_regs = strtol(num_registers, NULL, 16);

    // Calculate the length of the APDU
    int apdu_len = 7 + num_regs * 2;

    // Create the APDU buffer
    unsigned char apdu[256];
    apdu[0] = 0x10;
    apdu[1] = (start_reg >> 8) & 0xFF;
    apdu[2] = start_reg & 0xFF;
    apdu[3] = (num_regs >> 8) & 0xFF;
    apdu[4] = num_regs & 0xFF;
    apdu[5] = num_regs * 2;

    // Assuming 'values' is a hexadecimal string, you should convert it to binary data here.

    // Send the Modbus request
    unsigned char APDU_R[256];
    send_modbus_request(server_address, port, apdu, apdu_len, APDU_R);

    // Check if error response and, if error, return error code.
    if (APDU_R[0] != 16) {
        return APDU_R[1];
    }

    // Return the number of written registers
    int num_written_registers = (APDU_R[apdu_len - 2] << 8) | APDU_R[apdu_len - 1];
    return num_written_registers;
}

int read_holding_regs(const char* server_address, int port, const char* start_register, const char* num_registers, unsigned char* values) {
    // Convert start_register and num_registers from hexadecimal strings to integers
    int start_reg = strtol(start_register, NULL, 16);
    int num_regs = strtol(num_registers, NULL, 16);

    // Calculate the length of the APDU
    int apdu_len = 6;

    // Create the APDU buffer
    unsigned char apdu[256];
    apdu[0] = 0x03;
    apdu[1] = (start_reg >> 8) & 0xFF;
    apdu[2] = start_reg & 0xFF;
    apdu[3] = (num_regs >> 8) & 0xFF;
    apdu[4] = num_regs & 0xFF;

    // Send the Modbus request
    unsigned char APDU_R[256];
    send_modbus_request(server_address, port, apdu, apdu_len, APDU_R);

    // Assuming APDU_R contains the response data in binary format
    if (APDU_R == 0) {
        int num_read_registers = APDU_R[1] / 2;
        
        for (int i = 0; i < num_read_registers; i++) {
            values[i] = APDU_R[2 + 2 * i];
            values[i + 1] = APDU_R[3 + 2 * i];
        }
        return num_read_registers;
    }

    return -1;
}