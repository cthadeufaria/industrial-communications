


void send_modbus_request (server_addr, port, APDU, APDU_len, APDU_R) {
    // generates TI (trans.ID →sequence number)
    // assembles PDU = APDU(SDU) + MBAP
    // opens TCP client socket and connects to server (*)
    // write (fd, PDU, PDUlen) // sends Modbus TCP PDU
    // read (fd, PDU_R, PDU_Rlen) // response o timeout
    // if response, remove MBAP, PDU_R → APDU_R
    // closes TCP client socket with server (*)
    // returns: APDU_R and 0 – ok, <0 – error (timeout)
}