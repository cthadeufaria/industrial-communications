#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h> // for inet_aton
#include <unistd.h>

#define MYPORT 502
#define BUF_LEN 128 // Adjust the buffer size as needed

int main() {
    int sock, len;
    struct sockaddr_in serv;
    socklen_t addlen = sizeof(serv);
    char buf[BUF_LEN];

    // Create a socket
    sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        perror("Socket creation failed");
        return 1;
    }

    // Configure the server address
    serv.sin_family = AF_INET;
    serv.sin_port = htons(MYPORT);
    if (inet_aton("127.0.0.1", &serv.sin_addr) == 0) {
        perror("Invalid IP address");
        close(sock);
        return 1;
    }

    // Connect to the server
    if (connect(sock, (struct sockaddr *)&serv, addlen) == -1) {
        perror("Connection failed");
        close(sock);
        return 1;
    }

    // Input data to send
    printf("Enter data to send: ");
    if (fgets(buf, sizeof(buf), stdin) == NULL) {
        perror("Input error");
        close(sock);
        return 1;
    }

    // Send data to the server
    len = send(sock, buf, strlen(buf), 0);
    if (len == -1) {
        perror("Send failed");
        close(sock);
        return 1;
    }

    printf("Data sent successfully.\n");

    // Close the socket
    close(sock);

    return 0;
}
