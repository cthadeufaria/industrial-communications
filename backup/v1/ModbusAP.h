#ifndef MODBUSAP_H
#define MODBUSAP_H

#include <stdint.h>

// Modbus function codes
#define MODBUS_READ_HOLDING_REGISTER 0x03
#define MODBUS_WRITE_MULTIPLE_REGISTERS 0x10

// Function to read a holding register
int modbus_read_holding_register(int sockfd, uint8_t slave_address, uint16_t register_address, uint16_t *value);

// Function to write multiple registers
int modbus_write_multiple_registers(int sockfd, uint8_t slave_address, uint16_t start_address, uint16_t num_registers, const uint16_t *data);

#endif
