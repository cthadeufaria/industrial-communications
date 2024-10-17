#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

#define UNIT_ID 1

extern uint16_t TI;

int send_modbus_request (char*, int, uint8_t*, int, uint8_t*);