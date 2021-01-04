#include <sys/socket.h>
#include <endian.h>
#include <errno.h>

#include <olaf/olaf.h>

/* Helper functions to communicate with olaf daemon */

static ssize_t olaf_send(sock_t socket, olaf_code_t code, const void *arg)
{
	int res;
	struct olaf_request req;

	req.code = htobe64(code);

	res = send(socket, &req, sizeof(req), 0);
	if (res != sizeof(req))
		return -errno;

	if (OLAF_COMMAND_PERMS(code) & OLAF_WRITE) {
		res = send(socket, arg, OLAF_COMMAND_ARGS_SIZE(code), 0);
		if (res != OLAF_COMMAND_ARGS_SIZE(code))
			return -errno;
	}

	return 0;
}

static ssize_t olaf_recv(sock_t socket, void *arg)
{
	int res;
	struct olaf_request req;

	res = recv(socket, &req, sizeof(req), 0);
	if (res != sizeof(req))
		return -errno;

	req.code = be64toh(req.code);

	if (OLAF_COMMAND_PERMS(req.code) & OLAF_READ) {
		res = recv(socket, arg, OLAF_COMMAND_ARGS_SIZE(req.code), 0);
		if (res != OLAF_COMMAND_ARGS_SIZE(req.code))
			return -errno;
	}

	return 0;
}

ssize_t olaf_call(sock_t socket, olaf_code_t code, void *arg)
{
	ssize_t res;

	res = olaf_send(socket, code, arg);
	if (res)
		return res;

	res = olaf_recv(socket, arg);
	if (res)
		return res;

	return 0;
}
