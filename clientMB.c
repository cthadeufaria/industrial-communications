#include <stdio.h>
#include <stdint.h>
#include "modbusAP.h"
#include "modbusTCP.h"

#define MODBUS_TCP_PORT 502
#define MAX_REGISTERS 123

// Nao deve imprimir nada
//O que o programa deve fazer:
    // escrever/ler registros modbus
    // definir o endereco inicial dos registros
    // definir o numero de registros
    // definir o endereco ip do servidor
    // definir a porta do servidor
    // deve detectar e identificar excessoes modbus (imprimir erro modbus de resposta)
    // deve detetar e identificar erros nas funcoes e/ou tcp/ip (imprimir)
    // identificar campos nos PDU's (da camada modbusAP e modbusTCP) (2nd camada) (imprimir) ex: imprimir campo de numero de registros a gravar


int main() {
    char server_address[] = "127.0.0.1";
    // char server_address[] = "10.227.113.21";
    int port = MODBUS_TCP_PORT;
    uint16_t st_r=5, n_r=3, val[MAX_REGISTERS];

    val[0] = 0x0001;
    val[1] = 0x0002;
    val[2] = 0x0003;

    if (write_multiple_registers(server_address, port, st_r, n_r, val) < 0) {
        perror("clientMB.c: Error: write_multiple_registers\n");
        return 1;
    }
    else {
        printf("clientMB.c: write_multiple_registers: OK\n");
    }

    // if (read_holding_registers(server_address, port, st_r, n_r, val) < 0) {
    //     perror("clientMB.c: Error: read_holding_registers\n");
    //     return 1;
    // }
    // else {
    //     printf("clientMB.c: read_holding_registers: OK\n");
    // }

    return 0;
}