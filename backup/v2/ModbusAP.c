#include <stdio.h>
#include "ModbusAP.h"
#include "ModbusTCP.h"

int write_multiple_regs(const char *server_address, int port, uint16_t start_register, uint16_t num_registers, const uint16_t *values) {
    // Check consistency of parameters
    if (num_registers <= 0 || values == NULL) {
        return -1;
    }

    // Assemble APDU (MODBUS PDU)
    uint8_t apdu[256]; // Adjust the buffer size as needed
    // Fill in the APDU for a Modbus write operation (function code 0x10)
    apdu[0] = 0x10; // Function code for writing multiple registers
    apdu[1] = start_register >> 8; // High byte of starting register address
    apdu[2] = start_register & 0xFF; // Low byte of starting register address
    apdu[3] = num_registers >> 8; // High byte of number of registers to write
    apdu[4] = num_registers & 0xFF; // Low byte of number of registers to write
    apdu[5] = num_registers * 2; // Number of bytes to follow (2 bytes per register)

    // Copy the values to the APDU
    for (int i = 0; i < num_registers; i++) {
        apdu[6 + 2 * i] = values[i] >> 8; // High byte of register value
        apdu[7 + 2 * i] = values[i] & 0xFF; // Low byte of register value
    }

    // Send the Modbus request
    uint8_t apdu_response[256]; // Adjust the buffer size as needed
    int apdu_len = sizeof(apdu); // Calculate the actual APDU length
    int response_len = send_modbus_request(server_address, port, apdu, apdu_len, apdu_response);

    // Check the response (APDU_R or error_code)
    if (response_len < 0) {
        return -1;
    }

    // Print the response
    printf("Response: ");
    for (int i = 0; i < response_len; i++) {
        printf("%02X ", apdu_response[i]);
    }
    printf("\n");

    // Return the number of written registers or an error code
    return num_registers; // Replace with actual result
}

int read_h_regs(const char *server_address, int port, uint16_t start_register, uint16_t num_registers, uint16_t *values) {
    // Check consistency of parameters
    if (num_registers <= 0 || values == NULL) {
        return -1;
    }

    // Assemble APDU (MODBUS PDU)
    uint8_t apdu[256]; // Adjust the buffer size as needed
    // Fill in the APDU for a Modbus read operation (function code 0x03)
    apdu[0] = 0x03; // Function code for reading holding registers
    apdu[1] = start_register >> 8; // High byte of starting register address
    apdu[2] = start_register & 0xFF; // Low byte of starting register address
    apdu[3] = num_registers >> 8; // High byte of number of registers to read
    apdu[4] = num_registers & 0xFF; // Low byte of number of registers to read

    // Send the Modbus request
    uint8_t apdu_response[256]; // Adjust the buffer size as needed
    int apdu_len = sizeof(apdu); // Calculate the actual APDU length
    int response_len = send_modbus_request(server_address, port, apdu, apdu_len, apdu_response);

    // Check the response (APDU_R or error_code)
    if (response_len < 0) {
        return -1;
    }

    // Print the response
    printf("Response: ");
    for (int i = 0; i < response_len; i++) {
        printf("%02X ", apdu_response[i]);
    }
    printf("\n");

    // Return the number of read registers or an error code
    return num_registers; // Replace with actual result
}
