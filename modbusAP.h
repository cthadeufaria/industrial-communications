#include <stdio.h>
#include <stdint.h>

#define MAX_APDU 253

int write_multiple_registers (char*, int, uint16_t, uint16_t, uint16_t*);