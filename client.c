#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ModbusAP.h"

int main() {
    int MYPORT = 502;
    const char* MYIP = "127.0.0.1";

    // Simulated values for testing
    const char* function_code = "10";
    const char* start_register = "0000";
    const char* num_registers = "0002";
    const char* values = "0400070007";

    int result = write_multiple_regs(MYIP, MYPORT, start_register, num_registers, values);

    if (result < 0) {
        printf("Error: %d\n", result);
    } else {
        printf("Number of written registers: %d\n", result);
    }

    return 0;
}