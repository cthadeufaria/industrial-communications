import socket
from ModbusAP import read_holding_regs, write_multiple_regs

def main():
    MYPORT = 502
    BUF_LEN = 128
    MYIP = "127.0.0.1"

    # sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    # server_address = (MYIP, MYPORT)
    # sock.connect(server_address)

    # buf = input("Enter resquest message to send: ")
    buf = '100000000204ABCDEF01'
    function_code = buf[:2]

    if function_code == '10':
        start_register = buf[2:6]
        num_registers = buf[6:10]
        values = buf[10:]
        # Send data to the server
        result = write_multiple_regs(MYIP, MYPORT, start_register, num_registers, values)
    
    elif function_code == '03':
        start_register = buf[2:6]
        num_registers = buf[6:10]
        values = buf[10:]
        # Send data to the server
        result = write_multiple_regs(MYIP, MYPORT, start_register, num_registers, values)

    if result >= 0:
        print(f"Successfully wrote {result} registers.")
    else:
        print("Error writing registers.")

    # sock.close()


while __name__ == '__main__':
    main()