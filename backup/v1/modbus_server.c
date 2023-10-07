#include <stdio.h>
#include <modbus.h>

int main() {
    modbus_t *ctx;
    uint8_t *query;
    int listen_socket;
    int rc;

    // Create a Modbus context and set up a Modbus TCP server
    ctx = modbus_new_tcp("127.0.0.1", 20200);
    modbus_set_debug(ctx, TRUE); // Enable debugging

    // Listen for incoming connections
    listen_socket = modbus_tcp_listen(ctx, 1);
    modbus_tcp_accept(ctx, &listen_socket);

    while (1) {
        query = NULL;

        // Handle incoming Modbus requests
        rc = modbus_receive(ctx, &query);
        if (rc == -1) {
            fprintf(stderr, "Error receiving Modbus request\n");
            break;
        }

        // Respond to the Modbus request
        if (query) {
            modbus_reply(ctx, query, rc, ctx);
            free(query);
        }
    }

    // Clean up and exit
    modbus_tcp_close(ctx);
    modbus_free(ctx);

    return 0;
}
