#ifndef MODBUSAP_H
#define MODBUSAP_H

#include <stdint.h>

// Modbus function codes
#define MODBUS_READ_HOLDING_REGISTER 0x03
#define MODBUS_WRITE_MULTIPLE_REGISTERS 0x10

// Function to read holding registers
int read_h_regs(const char *server_address, int server_port, uint16_t start_register, uint16_t num_registers, uint16_t *values);

// Function to write multiple registers
int write_multiple_regs(const char *server_address, int server_port, uint16_t start_register, uint16_t num_registers, const uint16_t *values);

#endif
