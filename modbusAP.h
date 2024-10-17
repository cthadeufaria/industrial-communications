#include <stdio.h>
#include <stdint.h>



int write_multiple_registers (char*, int, uint16_t, uint16_t, uint16_t*);
int read_holding_registers (char*, int, uint16_t, uint16_t, uint16_t*);