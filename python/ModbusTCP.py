import socket
# from helpers import Transaction

def send_modbus_request(ip, port, APDU, APDUlen):
    protocol_identifier = 0x0000
    unitID = 0x11 # test unitID = 51
    
    # Generate transaction ID.
    # transaction_id = Transaction().id
    transaction_id = 0x0001

    try:

        # Assembles PDU = Mbap header + APDU.
        mbap = bytearray()
        mbap.extend([(transaction_id >> 8) & 0xFF, transaction_id & 0xFF])
        mbap.extend([(protocol_identifier >> 8) & 0xFF, protocol_identifier & 0xFF])
        mbap.extend([((APDUlen + 1) >> 8) & 0xFF, (APDUlen + 1) & 0xFF])
        mbap.append(unitID)
        
        # alternative: mbap = struct.pack(">HHHH", transaction_id, 0, APDUlen + 1, 0)

        PDU = mbap + APDU

        # Open TCP client socket and connect to server.
        fd = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        server_addr = (ip, port)
        fd.connect(server_addr)
        
        # Send Modbus TCP PDU.
        fd.send(PDU)

        # Get server response.
        PDU_R = fd.recv(APDUlen + 8)

        # Remove MBAP and get APDU response.
        if PDU_R:
            APDU_R = PDU_R[7:]
            print('PDU response:', PDU_R)
            print('APDU response:', APDU_R)
            result = 0

        else:
            APDU_R = b''

    except socket.error as e:
        print(f"Socket error: {e}")
        APDU_R = b''
        result = -1

    # Close TCP client socket with server.
    finally:
        fd.close()

    # return APDU and response status code.
    return APDU_R, result
