#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ModbusAP.h"

int main() {
    int MYPORT = 502;
    const char* MYIP = "127.0.0.1";

    // Simulated values for testing writing multiple registers
    // const char* function_code = "10";
    // const char* start_register = "0000";
    // const char* num_registers = "0002";
    // const char* values = "0400070007";

    // const char* function_code = "03";
    const char* start_register = "0073";
    const char* num_registers = "0001";
    const char* values = "020041";
    
    unsigned char A[256];
    unsigned char B[256];
    unsigned char C[256];

    int result = write_multiple_regs(MYIP, MYPORT, start_register, num_registers, values);

    const char* start_registerA = "0074";
    const char* num_registersA = "0004";

    int resultA = read_holding_regs(MYIP, MYPORT, start_registerA, num_registersA, &A);

    const char* start_registerB = "0078";
    const char* num_registersB = "0001";

    int resultB = read_holding_regs(MYIP, MYPORT, start_registerB, num_registersB, &B);

    if (B == 0) {
        C[0] = 0x99;
        C[1] = 0x99;
    }

    else if (B != 0) {
        C[0] = ((A[2] + A[6]) >> 8) & 0xFF;
        C[1] = (A[3] + A[7]) & 0xFF;
    }
    
    const char* start_registerC_1 = "0079";
    const char* num_registersC_1 = "0001";

    char hexString1[20];
    char hexString2[20];

    sprintf(hexString1, "%X", C[0]);
    sprintf(hexString2, "%X", C[1]);
    const char* C_value = strcat(hexString1, hexString2);

    int resultC1 = write_multiple_regs(MYIP, MYPORT, start_registerC_1, num_registersC_1, &C_value);

    const char* MYIP_C = "127.0.0.1";
    const char* start_registerC_2 = "0080";
    const char* num_registersC_2 = "0001";

    int resultC2 = write_multiple_regs(MYIP_C, MYPORT, start_registerC_2, num_registersC_2, &C_value);

    printf(result);

    return 0;
}