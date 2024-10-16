#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <string.h>
#include <stdio.h>

#define PORT 22222
#define IP "127.0.0.1"
#define BUF_LEN 1024


int main(void) {
    int sock;
    struct sockaddr_in sad_loc;
    char buf[BUF_LEN];
    int conn, sent, rec, closed, shut;

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("socket");
        return 1;
    }

    sad_loc.sin_family = PF_INET;
    sad_loc.sin_port = htons(PORT);
    inet_aton(IP, &sad_loc.sin_addr);

    conn = connect(sock, (struct sockaddr *)&sad_loc, sizeof(sad_loc));
    if (conn < 0) {
        perror("connect");
        return 1;
    }

    scanf("%s", buf);
    // write_multiple_registers

    sent = send(sock, buf, strlen(buf)+1, 0);
    if (sent < 0) {
        perror("send");
        return 1;
    }

    rec = recv(sock, buf, BUF_LEN, 0);
    if (rec < 0) {
        perror("recv");
        return 1;
    }

    printf("Received: %s\n", buf);

    closed = close(sock);
    if (closed < 0) {
        perror("close");
        return 1;
    }

    return 0;
}