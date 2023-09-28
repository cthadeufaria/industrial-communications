#include "ModbusAP.h"
#include "CRC.h" // Include the CRC header
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

// Modbus application layer function to read a holding register
int modbus_read_holding_register(int sockfd, uint8_t slave_address, uint16_t register_address, uint16_t *value) {
    // Modbus request packet for reading a holding register
    uint8_t request[8];
    request[0] = slave_address;            // Slave address
    request[1] = MODBUS_READ_HOLDING_REGISTER; // Function code
    request[2] = register_address >> 8;    // Starting register address (high byte)
    request[3] = register_address & 0xFF;  // Starting register address (low byte)
    request[4] = 0x00;                     // Number of registers to read (high byte)
    request[5] = 0x01;                     // Number of registers to read (low byte)

    // Calculate and set the CRC checksum for the request
    uint16_t crc = calculate_crc(request, 6);
    request[6] = crc & 0xFF;               // CRC (low byte)
    request[7] = (crc >> 8) & 0xFF;        // CRC (high byte)

    // Send the Modbus request to the server
    if (send(sockfd, request, 8, 0) == -1) {
        perror("Failed to send Modbus request");
        return -1; // Return an error code
    }

    // Receive and validate the Modbus response
    uint8_t response[256]; // Adjust the buffer size as needed
    ssize_t bytes_received = recv(sockfd, response, sizeof(response), 0);

    if (bytes_received == -1) {
        perror("Failed to receive Modbus response");
        return -1; // Return an error code
    } else if (bytes_received < 4) {
        fprintf(stderr, "Invalid Modbus response length\n");
        return -1; // Return an error code
    }

    // Check the response's slave address and function code
    if (response[0] != slave_address || response[1] != MODBUS_READ_HOLDING_REGISTER) {
        fprintf(stderr, "Invalid Modbus response\n");
        return -1; // Return an error code
    }

    // Extract the register value from the response
    *value = (response[2] << 8) | response[3];

    return 0; // Return 0 for success
}

// Modbus application layer function to write multiple registers
int modbus_write_multiple_registers(int sockfd, uint8_t slave_address, uint16_t start_address, uint16_t num_registers, const uint16_t *data) {
    // Modbus request packet for writing multiple registers
    uint8_t request[256]; // Adjust the buffer size as needed
    request[0] = slave_address;                  // Slave address
    request[1] = MODBUS_WRITE_MULTIPLE_REGISTERS; // Function code
    request[2] = start_address >> 8;             // Starting register address (high byte)
    request[3] = start_address & 0xFF;           // Starting register address (low byte)
    request[4] = num_registers >> 8;             // Number of registers to write (high byte)
    request[5] = num_registers & 0xFF;           // Number of registers to write (low byte)
    request[6] = num_registers * 2;              // Number of bytes to follow

    // Copy the data to the request packet
    for (int i = 0; i < num_registers; i++) {
        request[7 + 2 * i] = data[i] >> 8;      // Data (high byte)
        request[8 + 2 * i] = data[i] & 0xFF;    // Data (low byte)
    }

    // Calculate and set the CRC checksum for the request
    uint16_t crc = calculate_crc(request, 7 + 2 * num_registers);
    request[7 + 2 * num_registers] = crc & 0xFF;          // CRC (low byte)
    request[8 + 2 * num_registers] = (crc >> 8) & 0xFF;   // CRC (high byte)

    // Send the Modbus request to the server
    if (send(sockfd, request, 9 + 2 * num_registers, 0) == -1) {
        perror("Failed to send Modbus request");
        return -1; // Return an error code
    }

    // Receive and validate the Modbus response
    uint8_t response[256]; // Adjust the buffer size as needed
    ssize_t bytes_received = recv(sockfd, response, sizeof(response), 0);

    if (bytes_received == -1) {
        perror("Failed to receive Modbus response");
        return -1; // Return an error code
    } else if (bytes_received < 4) {
        fprintf(stderr, "Invalid Modbus response length\n");
        return -1; // Return an error code
    }

    // Check the response's slave address and function code
    if (response[0] != slave_address || response[1] != MODBUS_WRITE_MULTIPLE_REGISTERS) {
        fprintf(stderr, "Invalid Modbus response\n");
        return -1; // Return an error code
    }

    // Check the response's byte count
    uint8_t byte_count = response[2];
    if (byte_count != 4) {
        fprintf(stderr, "Invalid byte count in Modbus response\n");
        return -1; // Return an error code
    }

    // Extract the starting address and quantity of registers written from the response
    uint16_t starting_address = (response[3] << 8) | response[4];
    uint16_t quantity_written = (response[5] << 8) | response[6];

    if (starting_address != start_address || quantity_written != num_registers) {
        fprintf(stderr, "Mismatch in starting address or quantity of registers written\n");
        return -1; // Return an error code
    }

    return 0; // Return 0 for success
}

// Helper function to calculate the CRC checksum
uint16_t calculate_crc(const uint8_t *data, int length) {
    uint16_t crc = 0xFFFF;
    for (int i = 0; i < length; i++) {
        crc ^= data[i];
        for (int j = 0; j < 8; j++) {
            if (crc & 0x01) {
                crc >>= 1;
                crc ^= 0xA001;
            } else {
                crc >>= 1;
            }
        }
    }
    return crc;
}
