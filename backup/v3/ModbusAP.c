


int Write_multiple_regs(const char *server_add, int port, uint16_t st_r, uint16_t n_r, const uint16_t *val) {
    // Check consistency of parameters
    if (n_r <= 0 || val == NULL) {
        return -1; // Return an error code for invalid parameters
    }

    // Assemble APDU (MODBUS PDU)
    uint8_t APDU[256]; // Adjust the buffer size as needed

    // Fill in the APDU for a Modbus write operation (function code 0x10)
    APDU[0] = 0x10; // Function code for writing multiple registers
    APDU[1] = st_r >> 8; // High byte of starting register address
    APDU[2] = st_r & 0xFF; // Low byte of starting register address
    APDU[3] = n_r >> 8; // High byte of number of registers to write
    APDU[4] = n_r & 0xFF; // Low byte of number of registers to write
    APDU[5] = n_r * 2; // Number of bytes to follow (2 bytes per register)

    // Copy the values to the APDU
    for (int i = 0; i < n_r; i++) {
        APDU[6 + 2 * i] = val[i] >> 8; // High byte of register value
        APDU[7 + 2 * i] = val[i] & 0xFF; // Low byte of register value
    }

    // Send the Modbus request and receive the response
    uint8_t APDU_R[256]; // Adjust the buffer size as needed
    int response_len = send_modbus_request(server_add, port, APDU, sizeof(APDU), APDU_R);

    if (response_len < 0) {
        return response_len; // Return the error code if Send_Modbus_request fails
    }

    // Check the response (APDU_R or error_code) - Implement your response handling logic here

    // Return the number of written registers or an appropriate error code
    return num_written_registers; // Replace with actual result or error code
}