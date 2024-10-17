#include <stdio.h>
#include <stdint.h>
#include "modbusAP.h"

#define BUFFERSIZE 256 // 253
#define READ 3
#define WRITE 16
#define NUMERO_MAX_WRITE 123
#define NUMERO_MAX_READ 125// esta nos slides na parte function code na parte numero


int write_multiple_registers (char *server_addr, int port, uint16_t st_r, uint16_t n_r, uint16_t *val) {
    uint8_t apdu[MAX_APDU], APDU_R[5];
    int apdu_len;

    // check consistency of parameters
    if (server_addr == NULL || port <= 0 || val == NULL || n_r < 1 || n_r > 123) {
        fprintf(stderr, "Invalid parameters\n");
        return -1;
    }

    // assembles APDU (MODBUS PDU)
    apdu[0] = 0x10; // Function code for Write Multiple Registers
    apdu[1] = (uint8_t) (st_r >> 8); // Starting address high byte
    apdu[2] = (uint8_t) st_r; // Starting address low byte
    apdu[3] = (uint8_t) (n_r >> 8); // Quantity of registers high byte
    apdu[4] = (uint8_t) n_r; // Quantity of registers low byte
    apdu[5] = (uint8_t) n_r * 2; // Byte count

    for (int i = 0; i < n_r; i++) {
        apdu[6 + 2 * i] = (uint8_t) (val[i] >> 8); // Register value high byte
        apdu[7 + 2 * i] = (uint8_t) val[i]; // Register value low byte
    }

    apdu_len = 6 + 2 * n_r;
    
    // debug: print apdu
    printf("APDU: ");
    for (int i = 0; i < 6 + 2 * n_r; i++) {
        printf("%.2x ", apdu[i]);
    }
    printf("\n");

    printf("APDUlen: %d\n", apdu_len);

    // Send_Modbus_request (server_add,port,APDU,APDUlen,APDU_R)
    int result = send_modbus_request(server_addr, port, apdu, apdu_len, APDU_R);
    if (result < 0) {
        fprintf(stderr, "Failed to send Modbus request\n");
        return -2;
    }

    // Process the response in APDU_R if needed
    // Check if the response function code matches the request function code
    if (APDU_R[0] & 0x80) {
        fprintf(stderr, "Modbus error\n");
        return -APDU_R[1];
    }

    // // Check if the response contains the correct starting address and quantity of registers
    // uint16_t resp_st_r = (APDU_R[1] << 8) | APDU_R[2];
    // uint16_t resp_n_r = (APDU_R[3] << 8) | APDU_R[4];

    // if (resp_st_r != st_r || resp_n_r != n_r) {
    //     fprintf(stderr, "Mismatch in response starting address or quantity of registers\n");
    //     return -1;
    // }

    // checks the reponse (APDU_R or error_code)
    // returns: number of written registers – ok, <0 – error
    return n_r;
}


int read_holding_registers(char *server_add,unsigned int port, uint16_t st_r, uint16_t n_r, uint16_t *val )
{
    // check consistency of parameters
    // assembles APDU (MODBUS PDU)
    // Send_Modbus_request (server_add,port,APDU,APDUlen,APDU_R)
    // checks the reponse (APDU_R or error_code)
    // returns: number of read registers – ok, <0 – error
    if(st_r==0 || st_r<0)
    {
        //printf("Endereço inválido\n");
        return -50;
    }
    uint16_t new_str=st_r-1;
    uint8_t APDU[BUFFERSIZE], APDU_R[BUFFERSIZE];
    if(n_r>NUMERO_MAX_READ)
    {
        //printf("Excedeu o numero maximo dos registo\n");
        return -50;
    }
    uint16_t new_str1=st_r-1;
    APDU[0]=(uint8_t)(READ);
    APDU[1]=(uint8_t)(new_str1 >> 8);
    APDU[2]=(uint8_t)(new_str1 & 0b0000000011111111);
    APDU[3]=(uint8_t)(n_r >> 8);
    APDU[4]=(uint8_t)(n_r & 0b0000000011111111);
    uint16_t APDUlen=5; // duvida
    // ja esta feita a mensagem que se vai enviar
    int verifica_send_modbus1;
    verifica_send_modbus1=send_modbus_request(server_add, port, APDU,APDUlen, APDU_R); // manda para o TCP
    if(verifica_send_modbus1<0)
    {
        //printf(" Erro na parte send_modbus_request\n");
        return -50;
    }
    if (APDU_R[0]!=(uint8_t)(3))
    {
        if(APDU_R[1]==(uint8_t)(1))
        {
            //printf("Exception: ILLEGAL FUNCTION\n");   
            return -1;        
        }
        else if (APDU_R[1]==(uint8_t)(2))
        {
            //printf("Exception: ILLEGAL DATA ADDRESS\n");
            return -2;
        }
        else if (APDU_R[1]==(uint8_t)(3))
        {
            //printf("Exception: ILLEGAL DATA VALUE\n ");
            return -3;
        }
         else if (APDU_R[1]==(uint8_t)(4))
        {
            //printf("Exception: SERVER DEVICE FAILURE\n ");
            return -4;
        }
    }
    int indice1=1,i=0, flag_neg;
    uint16_t aux1, aux;
    for(i=0;i<(int)(APDU_R[1])/2;i++)
    {
        flag_neg=0;
        indice1=indice1+1;
        
         if((APDU_R[indice1] & 0b1000000000000000)==0b1000000000000000)
         {
            flag_neg=1;
         }
       /* val[i]=(uint16_t)(APDU_R[indice1]);
        indice1=indice1+1;
        val[i]=(uint16_t)(val[i]+APDU_R[indice1]);*/
       unsigned int binary = 0;
       for (int i = 15; i >= 0; i--) {
        int bit = (APDU_R[indice1] >> i) & 1;
        binary = (binary << 1) | bit;
    }
     // vamos ter em binario de 16 bits 
     binary = binary<<8;
    
     indice1++;
     unsigned int binary1 = 0;
       for (int i = 15; i >= 0; i--) {
        int bit = (APDU_R[indice1] >> i) & 1;
        binary1 = (binary1 << 1) | bit;
    }
    val[i]=(uint16_t)(binary|binary1);

        /*val[i]=val[i]<<8;
        val[i]=val[i] | 0b0000000011111111;// isto vai colocar nos 8 bits n altera e na parte coloca
        indice1=indice1+1;
        aux= (uint16_t)(APDU_R[indice1]);
        aux=aux | 0b1111111100000000; // coloca 111111 e o 8 bit menos significativos iguais*/
        
    }
    return i;
}