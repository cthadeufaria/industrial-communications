#include <stdio.h>
#include <stdint.h>
#include "modbusAP.h"
#include "modbusTCP.h"

#define MODBUS_TCP_PORT 502
#define MAX_REGISTERS 123

//O que o programa deve fazer:
    // Nao deve imprimir nada
    // escrever/ler registros modbus [ok]
    // definir o endereco inicial dos registros [ok]
    // definir o numero de registros [ok]
    // definir o endereco ip do servidor [ok]
    // definir a porta do servidor [ok]
    // deve detectar e identificar excessoes modbus (imprimir erro modbus de resposta) [ok]
    // deve detetar e identificar erros nas funcoes e/ou tcp/ip (imprimir) [ok]
    // identificar campos nos PDU's (da camada modbusAP e modbusTCP) (2nd camada) (imprimir) ex: imprimir campo de numero de registros a gravar


int main() {
    char server_address[] = "127.0.0.1";
    int port = MODBUS_TCP_PORT;
    uint16_t st_r=1, n_r=3, val[MAX_REGISTERS];

    val[0] = 0x0001;
    val[1] = 0x0010;
    val[2] = 0x0007;

    if (write_multiple_registers(server_address, port, st_r, n_r, val) < 0) {
        perror("clientMB.c: Error: write_multiple_registers\n");
        return -1;
    }

    if (read_holding_registers(server_address, port, st_r, n_r, val) < 0) {
        perror("clientMB.c: Error: read_holding_registers\n");
        return -1;
    }

    return 0;
}