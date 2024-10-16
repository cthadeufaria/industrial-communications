#include <stdio.h>
#include <stdint.h>
#include "modbusAP.h"
#include "modbusTCP.h"

#define MODBUS_TCP_PORT 502
#define MAX_REGISTERS 123



int main() {
    char server_address[] = "127.0.0.1";
    int port = MODBUS_TCP_PORT;
    uint16_t st_r=5, n_r=3, val[MAX_REGISTERS];

    val[0] = 0x0001;
    val[1] = 0x0002;
    val[2] = 0x0003;

    if (write_multiple_registers(server_address, port, st_r, n_r, val) < 0) {
        perror("clientMB.c: Error: write_multiple_registers\n");
        return 1;
    }
    else {
        printf("clientMB.c: write_multiple_registers: OK\n");
    }

    return 0;
}