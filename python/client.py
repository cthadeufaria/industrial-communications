from ModbusAP import read_holding_regs, write_multiple_regs
# from helpers import get_input

def main():
    MYPORT = 502
    MYIP = "127.0.0.1"

    # function_code, start_register, num_registers, values = get_input()
    function_code = '10'
    start_register = '0005'
    num_registers = '0003'
    values = '0400070007'

    if function_code == '10':
        result = write_multiple_regs(MYIP, MYPORT, start_register, num_registers, values)
        if result >= 0:
            print(f"Successfully wrote {result} registers.")
        else:
            print("Error writing registers.")
    
    elif function_code == '03':
        result, values = read_holding_regs(MYIP, MYPORT, start_register, num_registers)
        if result >= 0:
            print(f"Successfully read {result} registers:", values)
        else:
            print("Error reading registers.")


if __name__ == '__main__':
    main()