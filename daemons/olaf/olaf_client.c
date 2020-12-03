#include <bone/olaf.h>
#include <sys/socket.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <endian.h>

int main(int argc, char **argv)
{
	int master_socket;
	int res;
	struct sockaddr_in addr;
	struct olaf_login_args args = {"admin", "123456"};
	olaf_code_t code = htobe64(OLAF_LOGIN);

	master_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (master_socket < 0) {
		fprintf(stderr, "Failed to create master socket\n");
		exit(-1);
	}

	addr.sin_family = AF_INET;
	addr.sin_port = htons(1234);
	addr.sin_addr.s_addr = inet_addr("192.168.7.2");

	res = connect(master_socket, (struct sockaddr *) &addr, sizeof(addr));
	if (res < 0) {
		fprintf(stderr, "Failed to connect\n");
		exit(-1);
	}

	fprintf(stderr, "Conected\n");

	res = send(master_socket, &code, sizeof(code), 0);
	if (res != sizeof(code)) {
		fprintf(stderr, "Failed to send code\n");
		exit(-1);
	}

	fprintf(stderr, "Sended\n");

	res = send(master_socket, &args, sizeof(args), 0);
	if (res != sizeof(args)) {
		fprintf(stderr, "Failed to send args\n");
		exit(-1);
	}

	fprintf(stderr, "Sended\n");
}