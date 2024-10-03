from pymodbus.datastore import ModbusSlaveContext, ModbusServerContext
from pymodbus.datastore import ModbusSequentialDataBlock
from pymodbus.server import ServerTcp
from pymodbus.device import ModbusDeviceIdentification
import logging

# Configure the server logging
logging.basicConfig()
log = logging.getLogger()
log.setLevel(logging.DEBUG)

# Create a Modbus datastore with initial values
store = ModbusSlaveContext(
    di=ModbusSequentialDataBlock(0, [17] * 100),  # Discrete Inputs
    co=ModbusSequentialDataBlock(0, [17] * 100),  # Coils
    hr=ModbusSequentialDataBlock(0, [17] * 100),  # Holding Registers
    ir=ModbusSequentialDataBlock(0, [17] * 100)   # Input Registers
)

context = ModbusServerContext(slaves=store, single=True)

# Setup Modbus server identity (optional)
identity = ModbusDeviceIdentification()
identity.VendorName = 'Pymodbus'
identity.ProductCode = 'PM'
identity.VendorUrl = 'http://github.com/riptideio/pymodbus/'
identity.ProductName = 'Pymodbus Server'
identity.ModelName = 'Pymodbus Server'
identity.MajorMinorRevision = '3.0'

# Start Modbus TCP Server
if __name__ == "__main__":
    # Using the ServerTcp class to start the server
    server = ServerTcp(context=context, identity=identity, address=("localhost", 5020))
    server.serve_forever()
