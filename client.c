int main() {
    const char *server_address = "127.0.0.1"; // Replace with your server's IP address
    int port = 502;                           // Replace with your server's port

    char function_code[3];
    char start_register[5];
    char num_registers[5];
    char values[17];

    get_input('part', function_code, start_register, num_registers, values);

    if (strcmp(function_code, "10") == 0) {
        int result = write_multiple_regs(server_address, port, start_register, num_registers, values);
        if (result >= 0) {
            printf("Successfully wrote %d registers.\n", result);
        } else {
            printf("Error writing registers.\n");
        }
    } else if (strcmp(function_code, "03") == 0) {
        int result = read_holding_regs(server_address, port, start_register, num_registers);
        if (result >= 0) {
            printf("Successfully read %d registers.\n", result);
        } else {
            printf("Error reading registers.\n");
        }
    }

    return 0;
}

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

// Transaction class equivalent
struct Transaction {
    int id;
};

// Global variable to track the transaction ID
int transaction_id = 0;

// Transaction constructor equivalent
struct Transaction* create_transaction() {
    struct Transaction* transaction = (struct Transaction*)malloc(sizeof(struct Transaction));
    transaction->id = ++transaction_id;
    return transaction;
}

void destroy_transaction(struct Transaction* transaction) {
    free(transaction);
}

// Function to retrieve user input
void get_input(char method, char* function_code, char* start_register, char* num_registers, char* values) {
    if (method == 'part') {
        printf("\nEnter function code (1 bytes): ");
        scanf("%s", function_code);

        printf("\nEnter start register address (2 bytes): ");
        scanf("%s", start_register);

        printf("\nEnter number of registers (2 bytes): ");
        scanf("%s", num_registers);

        if (strcmp(function_code, "10") == 0) {
            strcpy(values, "00");
            int num_reg = strtol(num_registers, NULL, 16);
            for (int i = 0; i < num_reg; i++) {
                char register_value[3];
                printf("\nEnter register value %d (2 bytes): ", i + 1);
                scanf("%s", register_value);
                strcat(values, register_value);
            }
        }
    } else if (method == 'buf') {
        printf("Enter request message to send: ");
        scanf("%s", values);

        // Extract function code, start register, and num_registers
        strncpy(function_code, values, 2);
        function_code[2] = '\0';
        strncpy(start_register, values + 2, 4);
        start_register[4] = '\0';
        strncpy(num_registers, values + 6, 4);
        num_registers[4] = '\0';
    }
}

// Function to check consistency of parameters
int check_consistency(const char* start_register, const char* num_registers, const char* values) {
    int start_reg = strtol(start_register, NULL, 16);
    int num_reg = strtol(num_registers, NULL, 16);

    if (start_reg > 0xFFFF || num_reg > 0x007B || num_reg < 0x0001) {
        printf("Byte string is bigger than accepted.\n");
        return 0;
    }

    if (values != NULL) {
        int values_len = strlen(values);
        int expected_len = 2 * num_reg;

        if (values_len != expected_len) {
            printf("Number of registers byte is not consistent with values bytes given.\n");
            return 0;
        }
    }

    return 1;
}