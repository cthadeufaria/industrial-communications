


void write_multiple_registers (server_addr, port, st_r, n_r, val) {
    // check consistency of parameters
    // assembles APDU (MODBUS PDU)
    // Send_Modbus_request (server_add,port,APDU,APDUlen,APDU_R)
    // checks the reponse (APDU_R or error_code)
    // returns: number of written registers – ok, <0 – error
}

void read_holding_registers (server_addr, port, st_r, n_r, val) {
    // check consistency of parameters
    // assembles APDU (MODBUS PDU)
    // Send_Modbus_request (server_add,port,APDU,APDUlen,APDU_R)
    // checks the reponse (APDU_R or error_code)
    // returns: number of read registers – ok, <0 – error
}