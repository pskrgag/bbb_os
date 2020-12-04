#include <sys/socket.h>

#include <bone/olaf.h>

uint64_t olaf_call(sock_t socket, olaf_code_t code, void *args, size_t args_size)
{
	int res;

	res = send(socket, &code, sizeof(code), 0);
	if (res != sizeof(code))
		return -1;

	res = send(socket, args, args_size, 0);
	if (res != args_size)
		return -1;

	/* TODO: add recv here */
	/* but no commands now support answer */

	return res;
}
