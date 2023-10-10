#ifndef MODBUSTCP_H
#define MODBUSTCP_H

// Function to send a Modbus request over TCP/IP
void send_modbus_request(const char* ip, int port, const char* APDU, int APDUlen, char* APDU_R);

#endif  // MODBUSTCP_H
