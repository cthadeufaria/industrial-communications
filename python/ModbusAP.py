from ModbusTCP import send_modbus_request
# from helpers import check_consistency

def write_multiple_regs(server_address, port, start_register, num_registers, values):
    # Check consistency of parameters.
    # if not check_consistency(start_register, num_registers, values):
    #     return -1

    # Assemble APDU.
    start_register = int(start_register, 16)
    num_registers = int(num_registers, 16)
    apdu = bytearray()
    apdu.append(0x10)
    apdu.extend([(start_register >> 8) & 0xFF, start_register & 0xFF])
    apdu.extend([(num_registers >> 8) & 0xFF, num_registers & 0xFF])
    apdu.append(num_registers * 2)

    for i in range(0, int(values[0:2], 16) * 2, 4):
        apdu.extend([(int(values[2+i:6+i], 16) >> 8) & 0xFF, int(values[2+i:6+i], 16) & 0xFF])

    apdu_len = len(apdu)

    # Send request.
    APDU_R, result = send_modbus_request(server_address, port, apdu, apdu_len)

    # Check if error response and, if error, return error code.
    if APDU_R[0] != 16:
        return hex(APDU_R[1])

    # Return number of written registers
    if result == 0:
        num_written_registers = (APDU_R[-2] << 8) | APDU_R[-1]
        return num_written_registers        

    return -1

def read_holding_regs(server_address, port, start_register, num_registers):
    # Check consistency of parameters.
    # if not check_consistency(start_register, num_registers, values):
    #     return -1

    # Assemble APDU
    start_register = int(start_register, 16)
    num_registers = int(num_registers, 16)
    apdu = bytearray()
    apdu.append(0x03)
    apdu.extend([(start_register >> 8) & 0xFF, start_register & 0xFF])
    apdu.extend([(num_registers >> 8) & 0xFF, num_registers & 0xFF])

    apdu_len = len(apdu)

    # Send request.
    APDU_R, result = send_modbus_request(server_address, port, apdu, apdu_len)

    values = bytearray()

    # Check if error response and, if error, return error code.
    if APDU_R[0] != 16:
        return hex(APDU_R[1])

    # Return number of written registers
    if result == 0:
        num_read_registers = int(int(APDU_R[1]) / 2)
        
        for i in range(num_read_registers):
            values.append(APDU_R[2 + 2 * i])
            values.append(APDU_R[3 + 2 * i])
        
        return num_read_registers, values

    return -1

