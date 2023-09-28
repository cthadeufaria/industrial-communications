#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define MYPORT 20200
#define BUF_LEN 10

main () {
int sock, len;
struct sockaddr_in serv;

socklen_t addlen = sizeof(serv);

char buf[BUF_LEN];

sock = socket(PF_INET, SOCK_STREAM, 0);

serv.sin_family = AF_INET;

serv.sin_port = htons(MYPORT);

inet_aton("127.0.0.1",&serv.sin_addr);

connect(sock, (struct sockaddr *) &serv, addlen);

scanf("%s", buf);

len=send(sock, buf, strlen(buf)+1, 0);
}


// main() {
// 	server_add = ;
// 	port = ;
// 	st_r = ;
// 	n_r = ;
// 	val = xxxx
	
// 	Write_multiple.regs (server_add, port, st_r, 
// }