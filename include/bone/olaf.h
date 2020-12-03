#ifndef __OLAF_H__
#define __OLAF_H__

#include <bone/olaf_commands.h>

/* This header file provides olaf commands */

#define OLAF_MAX_LOGIN_NAME		50
#define OLAF_MAX_LOGIN_PASSWD		100

struct olaf_login_args {
	char name[OLAF_MAX_LOGIN_NAME];
	char passwd[OLAF_MAX_LOGIN_PASSWD];
} __packed;

#define OLAF_LOGIN	OLAF_TYPE_COMMAND(0, struct olaf_login_args)

#endif