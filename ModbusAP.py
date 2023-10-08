from ModbusTCP import send_modbus_request

# Modbus application layer function to write multiple registers
def write_multiple_regs(server_address, port, start_register, num_registers, values):
    # Convert hexadecimal string to integer
    start_register = int(start_register, 16)
    num_registers = int(num_registers, 16)

    if num_registers <= 0 or not values:
        return -1  # Error: Invalid parameters

    apdu = bytearray()
    apdu.append(0x10)
    apdu.extend([(start_register >> 8) & 0xFF, start_register & 0xFF])
    apdu.extend([(num_registers >> 8) & 0xFF, num_registers & 0xFF])
    apdu.append(num_registers * 2)

    for i in range(0, int(values[0:2]) * 2, 4):
        apdu.extend([(int(values[2+i:6+i], 16) >> 8) & 0xFF, int(values[2+i:6+i], 16) & 0xFF])  # Register value (high and low bytes)

    apdu_len = len(apdu)

    APDU_R, result = send_modbus_request(server_address, port, apdu, apdu_len)

    if result == 0:
        num_written_registers = (APDU_R[4] << 8) | APDU_R[5]
        return num_written_registers

    return -1

def read_holding_regs(server_address, port, start_register, num_registers, values):
    # Check consistency of parameters
    if num_registers <= 0 or not values:
        return -1  # Error: Invalid parameters

    # Assemble APDU (MODBUS PDU)
    apdu = bytearray()
    apdu.append(0x03)  # Function code for reading holding registers
    apdu.extend([(start_register >> 8) & 0xFF, start_register & 0xFF])  # Starting register address (high and low bytes)
    apdu.extend([(num_registers >> 8) & 0xFF, num_registers & 0xFF])  # Number of registers to read (high and low bytes)

    # Send the Modbus request and get the response
    APDU_R, result = send_modbus_request(server_address, port, apdu)

    # Check the response (APDU_R or error_code)
    if result == 0:
        # Calculate the number of read registers (byte 2 in the response)
        num_read_registers = APDU_R[2]
        
        # Extract and store the values from the response
        for i in range(num_read_registers // 2):
            register_value = (APDU_R[3 + 2 * i] << 8) | APDU_R[4 + 2 * i]
            values.append(register_value)
        
        return num_read_registers  # Number of read registers

    return -1  # Error

# Example usage:
# if __name__ == '__main__':
#     server_address = '127.0.0.1'
#     port = 502
#     start_register = 100
#     num_registers = 5
#     values = []

#     result = Read_h_regs(server_address, port, start_register, num_registers, values)

#     if result >= 0:
#         print(f"Successfully read {result} registers: {values}")
#     else:
#         print("Error reading registers.")

