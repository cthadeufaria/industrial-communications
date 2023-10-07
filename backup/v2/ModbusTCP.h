#ifndef MODBUSTCP_H
#define MODBUSTCP_H

#include <stdint.h>

// Function to send a Modbus request over TCP and receive the response
int send_modbus_request(const char *server_address, int port, const uint8_t *apdu, int apdu_len, uint8_t *apdu_response);

#endif
