# Industrial Communications FEUP class projects

**Project 01:**

**Two different Modbus/TCP libraries written in Python and C for a client containing the two following functions:**

**• 03 (0x03) Read Holding Register**

**• 16 (0x10) Write Multiple Registers**

#### Before starting, to make sure the port you're connecting to is available:

1. Check port status:

   ```
   sudo ufw status | grep 502
   ```

2. If no message is returned:

   ```
   sudo ufw allow 502/tcp && sudo ufw reload
   ```

#### To create a connection between the client and local server on port 502 and check traffic using Diagslave and Wireshark on Ubuntu 22.04.3 LTS x86_64 (given all the code in C is already compiled), run:

3. Create local Modbus TCP server on port 502 using diagslave:
    
    ```
    sudo ./diagslave-3.4/diagslave/x86_64-linux-gnu/diagslave -m tcp
    ```

4. Run the client to connect to local server created:
    
    ```
    sudo ./client
    ```

#### After the following message from the server: validateMasterIpAddr: accepting connection from 127.0.0.1, open Wireshark and follow the instructions below.

To use Wireshark to capture and analyze Modbus TCP traffic between your client and server, you can follow these steps:

5. **Install Wireshark**: If you haven't already installed Wireshark, you can do so by running the following command:

   ```
   sudo apt install wireshark
   ```

6. **Run Wireshark**: Open Wireshark from the terminal using the following command:

   ```
   sudo wireshark
   ```

   Note: You might need to run Wireshark with sudo privileges to capture network traffic.

7. **Select Network Interface**: In Wireshark, select the network interface that is connected to your Modbus communication. You should see a list of available interfaces. Choose the one that corresponds to your network connection.

8. **Capture Filter**: To capture only Modbus TCP traffic, you can set a capture filter. In the "Capture" menu, choose "Capture Filters." Create a new filter with the following expression:

   ```
   port 502
   ```

   This filter will capture traffic on port 502, which is the default port for Modbus TCP.

9. **Start Capture**: Click the "Start" button in Wireshark to begin capturing network traffic.

10. **Execute Modbus Transactions**: Run your Modbus client and perform the Modbus transactions you want to monitor.

11. **Stop Capture**: Once you've captured the desired traffic, go back to Wireshark and click the "Stop" button.

12. **Analyze Traffic**: You can now analyze the captured Modbus traffic in Wireshark. You can filter and inspect packets, view protocol details, and look for any issues or anomalies.

13. **Save Capture**: If you need to save the capture for further analysis or documentation, you can go to "File" > "Save" and save it in the desired format (e.g., PCAP).

Keep in mind that capturing network traffic with Wireshark may require administrative privileges (sudo). Additionally, be cautious when capturing traffic on a network, especially if it's a production environment, as it may introduce some overhead and privacy concerns. Always follow proper security and privacy guidelines when capturing network data.