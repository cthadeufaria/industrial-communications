import itertools

class Transaction():
    id_obj = itertools.count(1)

    def __init__(self):
        self.id = next(Transaction.id_obj)

def get_input(method='part'):
    
    if method == 'part':
        function_code = input("\nEnter function code (1 bytes): ")
        start_register = input("\nEnter start register address (2 bytes): ")
        num_registers = input("\nEnter number of registers (2 bytes): ")
        if function_code == '10':
            values = hex(int(num_registers, 16) * 2)[-2:].replace('x', '0')
            for i in range(int(num_registers)):
                values += input("\nEnter register value %s (2 bytes): " % str(i + 1))
        else:
            values = None
    
    elif method == 'buf':
        buf = input("Enter resquest message to send: ")
        # buf = '100000000204ABCDEF01'
        # buf = '0300000002'
        function_code = buf[:2]
        start_register = buf[2:6]
        num_registers = buf[6:10]
        if function_code == '10':
            values = buf[10:]
        else:
            values = None

    return function_code, start_register, num_registers, values

def check_consistency(start_register, num_registers, values=None):
    if (int(start_register, 16) > int('0xFFFF', 16)) | (int(num_registers, 16) > int('0x007B', 16)) | (int(num_registers, 16) < int('0x0001', 16)):
        print('Byte string bigger than accepted.')
        return False

    if values != None:
        if int(num_registers, 16) != len(values[2:]) / 4:
            print('Number of registers byte not consistent with values bytes given.')
            return False
        
    return True