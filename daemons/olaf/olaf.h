#ifndef __OLAF_PRIVATE_H
#define __OLAF_PRIVATE_H

#include <stdint.h>
#include <sys/socket.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <endian.h>
#include <netinet/tcp.h>

/* Constants */
#define DEFAULT_PORT		1234
#define IP			"192.168.7.2"
#define HOST_IP			"192.168.7.1"		/* over usb0 interface */
#define OLAF_FILE(file)		"/etc/olaf/" file

/* log macros */
#define log(level, format...)	syslog(level, format)
#define log_err(format...)	syslog(LOG_ERR, format)

#ifdef DEBUG
#define DBG(...)	log(LOG_ERR, __VA_ARGS__)
#else
#define DBG(...)
#endif

static inline int olaf_get_request(sock_t sock, struct olaf_request *req)
{
	int res;

	res = recv(sock, req, sizeof(*req), 0);
	if (res != sizeof(*req)) {
		log(LOG_ERR, "Failed to get code from user: %s\n", strerror(errno));
		return -1;
	}

	req->code = be64toh(req->code);
	log_err("Got code = %llu. GET_NAME_CODE = %llu\n", req->code, OLAF_GET_DEVICE_INFO);

	return 0;
}

static inline int __check_login(const struct olaf_login_args *args, const char *pass)
{
	/* TODO: make hash check */
	return !strcmp(args->passwd, pass);
}

#define LOG_FILE	OLAF_FILE("log.conf")
static inline int olaf_check_login(const struct olaf_login_args *args)
{
	FILE *file;
	char *line = NULL, name[OLAF_MAX_LOGIN_NAME] = {0}, pass[OLAF_MAX_LOGIN_PASSWD] = {0};
	size_t size;

	file = fopen(LOG_FILE, "r");
	if (!file) {
		log_err("Failed to open log file\n");
		return -1;
	}

	while (getline(&line, &size, file) != -1) {
		sscanf(line, "%s %s", name, pass);

		if (!strcmp(args->name, name)) {
			if (__check_login(args, pass))
				return 1;
			else
				return 0;
		}

		memset(name, 0, sizeof(name));
		memset(pass, 0, sizeof(pass));
	}

	return 0;
}

static inline int olaf_enable_keep_alive(sock_t sock)
{
	const static int yes = 1, idle = 1, interval = 1, maxpkt = 10;
	int res;

	res = setsockopt(sock, SOL_SOCKET, SO_KEEPALIVE, &yes, sizeof(yes));
	if (res)
		goto err;

	res = setsockopt(sock, IPPROTO_TCP, TCP_KEEPIDLE, &idle, sizeof(idle));
	if (res)
		goto err;

	res = setsockopt(sock, IPPROTO_TCP, TCP_KEEPINTVL, &interval, sizeof(interval));
	if (res)
		goto err;

	res = setsockopt(sock, IPPROTO_TCP, TCP_KEEPCNT, &maxpkt, sizeof(maxpkt));
	if (res)
		goto err;

	return 0;

err:
	return -1;
}

static inline int olaf_check_handshake(sock_t sock)
{
	/* TODO: add handshake check */
	return 0;
}


static int olaf_login(sock_t socket)
{
	int res;
	struct olaf_login_args args;
	olaf_code_t code;

	res = recv(socket, &args, sizeof(args), 0);
	if (res != sizeof(args)) {
		log_err("Errno = %s\n", strerror(errno));
		return -1;
	}

	return olaf_check_login(&args);
}

static inline size_t olaf_get_name(sock_t socket, const struct olaf_request *req, struct olaf_device_info *info)
{
	memset(info, 0, sizeof(info));
	strcpy(info->name, "BeagleBone Black");

	return sizeof(struct olaf_device_info);
}

#define OLAF_PRE_ERROR		-1
#define OLAF_LOGGED		0
#define OLAF_GOT_NAME		1

static inline int pre_connection(sock_t socket)
{
	struct olaf_request req;
	int res;
	void *ptr = NULL;
	size_t size = 0;

	res = olaf_get_request(socket, &req);
	if (res || req.code == OLAF_WRONG_CODE)
		goto error;

	/* only these commands can be first commands send by user */
	switch (req.code) {
	case OLAF_LOGIN:
		res = olaf_login(socket);
		if (!res) {
			res = OLAF_PRE_ERROR;
			break;
		}

		return OLAF_LOGGED;
	case OLAF_GET_DEVICE_INFO:
		ptr = malloc(sizeof(struct olaf_device_info));

		if (!ptr)
			return OLAF_PRE_ERROR;

		size = olaf_get_name(socket, &req, ptr);

		res = OLAF_GOT_NAME;
	}

	req.code = htobe64(req.code);

	res = send(socket, &req, sizeof(req), 0);
	if (res != sizeof(req)) {
		res = OLAF_PRE_ERROR;
		goto error;
	}

	log_err("Sended Request\n");

	res = send(socket, ptr, size, 0);
	if (res != size)
		res = OLAF_PRE_ERROR;

	log_err("Sended args\n");

error:
	free(ptr);
	return res;
}

#endif /* __OLAF_PRIVATE_H */