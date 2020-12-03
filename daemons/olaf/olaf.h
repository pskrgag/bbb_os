#ifndef __OLAF_PRIVATE_H
#define __OLAF_PRIVATE_H

#include <stdint.h>
#include <sys/socket.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <endian.h>

typedef int sock_t;

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

static inline olaf_code_t olaf_get_code(sock_t sock)
{
	olaf_code_t code;
	int res;

	res = recv(sock, &code, sizeof(code), 0);
	if (res != sizeof(code)) {
		log(LOG_ERR, "Failed to get code from user: %s\n", strerror(errno));
		return (olaf_code_t) -1;
	}

	return be64toh(code);
}

static inline int __check_login(const struct olaf_login_args *args, const char *pass)
{
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

#endif