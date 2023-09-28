#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "ModbusAP.h"
#include "ModbusTCP.h"

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 20200

int main() {
    int sockfd;
    uint16_t holding_register_value;
    uint16_t registers_to_write[] = {100, 200, 300};

    // Connect to the Modbus server
    if (modbus_tcp_connect(SERVER_IP, SERVER_PORT, &sockfd) != 0) {
        fprintf(stderr, "Failed to connect to the Modbus server.\n");
        return 1;
    }

    // Read holding register example
    if (modbus_read_holding_register(sockfd, 1, 0, &holding_register_value) != 0) {
        fprintf(stderr, "Failed to read holding register.\n");
    } else {
        printf("Holding Register Value: %u\n", holding_register_value);
    }

    // Write multiple registers example
    if (modbus_write_multiple_registers(sockfd, 1, 0, 3, registers_to_write) != 0) {
        fprintf(stderr, "Failed to write multiple registers.\n");
    } else {
        printf("Multiple registers written successfully.\n");
    }

    // Close the Modbus TCP connection
    modbus_tcp_close(sockfd);

    return 0;
}