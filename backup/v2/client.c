#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "ModbusAP.h"
#include "ModbusTCP.h"

int main() {
    char input[1024]; // Assuming a maximum input length of 1024 characters

    // Input the entire Modbus request message in hexadecimal format
    printf("Enter Modbus request message (hexadecimal): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        perror("Input error");
        return 1;
    }

    // Remove trailing newline characters, if any
    input[strcspn(input, "\n")] = '\0';

    // Convert the hexadecimal input into binary
    size_t input_length = strlen(input);
    if (input_length % 2 != 0) {
        printf("Invalid hexadecimal input\n");
        return 1;
    }

    size_t binary_length = input_length / 2;
    uint8_t *binary_input = (uint8_t *)malloc(binary_length);
    if (binary_input == NULL) {
        perror("Memory allocation failed");
        return 1;
    }

    for (size_t i = 0; i < input_length; i += 2) {
        if (sscanf(&input[i], "%2hhx", &binary_input[i / 2]) != 1) {
            printf("Invalid hexadecimal input\n");
            free(binary_input);
            return 1;
        }
    }

    // Configure the Modbus server address and port
    const char *server_address = "127.0.0.1"; // Modify as needed
    int server_port = 502; // Modify as needed

    // Send the Modbus request and receive the response
    uint8_t apdu_response[256]; // Adjust the buffer size as needed
    int response_length = send_modbus_request(server_address, server_port, binary_input, binary_length, apdu_response);

    if (response_length < 0) {
        printf("Modbus request failed\n");
        free(binary_input);
        return 1;
    }

    // Print the response in hexadecimal format
    printf("Response: ");
    for (int i = 0; i < response_length; i++) {
        printf("%02X", apdu_response[i]);
    }
    printf("\n");

    // Free allocated memory
    free(binary_input);

    return 0;
}
