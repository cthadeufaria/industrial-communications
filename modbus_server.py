from pymodbus.server.sync import StartTcpServer
from pymodbus.datastore import ModbusSequentialDataBlock
from pymodbus.datastore import ModbusServerContext

# Create a Modbus server context
store = ModbusServerContext(slaves=1, single=True)

# Create a data block with some values
block = ModbusSequentialDataBlock(0, [0]*100)

# Add the data block to the server context
store.register(block, 0)

# Start the Modbus TCP server
StartTcpServer(store, address=("127.0.0.1", 20200))
