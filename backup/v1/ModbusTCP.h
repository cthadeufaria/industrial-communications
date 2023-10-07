#ifndef MODBUSTCP_H
#define MODBUSTCP_H

#include <stdbool.h>

// Function to establish a Modbus TCP connection
int modbus_tcp_connect(const char *server_ip, int server_port, int *sockfd);

// Function to close a Modbus TCP connection
void modbus_tcp_close(int sockfd);

#endif
