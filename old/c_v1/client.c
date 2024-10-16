#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ModbusAP.h"

int main() {
    int MYPORT = 502;
    const char* MYIP = "127.0.0.1";

    // Simulated values for testing writing multiple registers
    const char* function_code = "10";
    const char* start_register = "0000";
    const char* num_registers = "0002";
    const char* values = "0400070007";

    // Simulated values for testing reading multiple registers
    // const char* function_code = "03";
    // const char* start_register = "0000";
    // const char* num_registers = "0002";
    // const char* values = NULL;

    // int result = write_multiple_regs(MYIP, MYPORT, start_register, num_registers, values);
    int result = read_holding_regs(MYIP, MYPORT, start_register, num_registers, &values);

    printf(result);

    return 0;
}