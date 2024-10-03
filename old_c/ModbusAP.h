#ifndef MODBUSAP_H
#define MODBUSAP_H

int write_multiple_regs(const char* server_address, int port, const char* start_register, const char* num_registers, const char* values);

#endif // MODBUSAP_H
// int read_holding_regs(const char* server_address, int port, const char* start_register, const char* num_registers, char* values);