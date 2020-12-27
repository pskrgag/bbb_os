#ifndef __OLAF_H__
#define __OLAF_H__

#include <bone/api/olaf_commands.h>

/* This header file provides olaf commands */

#define OLAF_KEEP_ALIVE		(238942743ULL)

typedef int sock_t;

struct olaf_request {
	olaf_code_t code;
	/* may be expanded */
} __packed;

/* ----------------------------------------------------------------------------------- */

#define OLAF_MAX_LOGIN_NAME		50
#define OLAF_MAX_LOGIN_PASSWD		100

typedef uint8_t login_perms;

struct olaf_login_args {
	char name[OLAF_MAX_LOGIN_NAME];
	char passwd[OLAF_MAX_LOGIN_PASSWD];
	login_perms perms;
} __packed;

#define OLAF_LOGIN			OLAF_COMMAND(0, struct olaf_login_args, OLAF_WRITE)

/* ----------------------------------------------------------------------------------- */

#define OLAF_MAX_DEVICE_NAME		20

struct olaf_device_info {
	char name[OLAF_MAX_DEVICE_NAME];
} __packed;

#define OLAF_GET_DEVICE_INFO		OLAF_COMMAND(1, struct olaf_device_info, OLAF_READ)

#endif
