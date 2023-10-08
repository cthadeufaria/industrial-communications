import socket

def send_modbus_request(ip, port, APDU, APDUlen):
    # Create a socket
    fd = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    try:
        transaction_id = 0x0001 # TODO: implement incremental logic.
        protocol_identifier = 0x0000
        unitID = 0x11

        # mbap = struct.pack(">HHHH", transaction_id, 0, APDUlen + 2, 0)
        mbap = bytearray()
        mbap.append((transaction_id >> 8) & 0xFF)
        mbap.append(transaction_id & 0xFF)
        mbap.extend([(protocol_identifier >> 8) & 0xFF, protocol_identifier & 0xFF])
        mbap.append((APDUlen + 1 >> 8) & 0xFF)
        mbap.append(APDUlen + 1 & 0xFF)
        mbap.append(unitID)

        PDU = mbap + APDU

        server_addr = (ip, port)
        fd.connect(server_addr)
        fd.send(PDU)

        PDU_R = fd.recv(APDUlen + 8)

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

    finally:
        fd.close()

    return APDU_R, result
