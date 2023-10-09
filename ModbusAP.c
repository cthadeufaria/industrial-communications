#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define UNIT_ID 0x11

int write_multiple_regs(const char *server_address, int port, const char *start_register, const char *num_registers, const char *values) {
    // Check consistency of parameters.
    // Note: You need to implement check_consistency function in C.
    if (!check_consistency(start_register, num_registers, values)) {
        return -1;
    }

    // Convert start_register and num_registers to integers.
    int start_reg = (int)strtol(start_register, NULL, 16);
    int num_regs = (int)strtol(num_registers, NULL, 16);

    // Assemble APDU.
    uint8_t apdu[256]; // Adjust the buffer size as needed.
    apdu[0] = 0x10;
    apdu[1] = (uint8_t)(start_reg >> 8);
    apdu[2] = (uint8_t)(start_reg & 0xFF);
    apdu[3] = (uint8_t)(num_regs >> 8);
    apdu[4] = (uint8_t)(num_regs & 0xFF);
    apdu[5] = (uint8_t)(num_regs * 2);

    // Parse and add values to APDU.
    int values_len = strlen(values);
    int j = 6; // Initialize j here
    for (int i = 0; i < values_len; i += 4, j += 2) {
        apdu[j] = (uint8_t)(strtol(&values[i], NULL, 16) >> 8);
        apdu[j + 1] = (uint8_t)(strtol(&values[i], NULL, 16) & 0xFF);
    }

    int apdu_len = j;

    // Send request.
    send_modbus_request(server_address, port, apdu, apdu_len);

    // Implement error handling as needed.
    // If no error, return the number of written registers.
    return -1;
}

int read_holding_regs(const char *server_address, int port, const char *start_register, const char *num_registers, const char *values) {
    // Check consistency of parameters.
    // Note: You need to implement check_consistency function in C.
    if (!check_consistency(start_register, num_registers, values)) {
        return -1;
    }

    // Convert start_register and num_registers to integers.
    int start_reg = (int)strtol(start_register, NULL, 16);
    int num_regs = (int)strtol(num_registers, NULL, 16);

    // Assemble APDU.
    uint8_t apdu[256]; // Adjust the buffer size as needed.
    apdu[0] = 0x03;
    apdu[1] = (uint8_t)(start_reg >> 8);
    apdu[2] = (uint8_t)(start_reg & 0xFF);
    apdu[3] = (uint8_t)(num_regs >> 8);
    apdu[4] = (uint8_t)(num_regs & 0xFF);

    int apdu_len = 5;

    // Send request.
    send_modbus_request(server_address, port, apdu, apdu_len);

    // Implement error handling as needed.
    // If no error, return the number of read registers and values.
    return -1;
}

// int main() {
//     const char *server_address = "127.0.0.1"; // Replace with your server's IP address
//     int port = 502;                           // Replace with your server's port

//     // Example usage:
//     const char *start_register = "0001";
//     const char *num_registers = "0002";
//     const char *values = "0102";

//     int write_result = write_multiple_regs(server_address, port, start_register, num_registers, values);
//     int read_result = read_holding_regs(server_address, port, start_register, num_registers);

//     // Handle the results as needed.

//     return 0;
// }
