#ifndef __IOCLT_H
#define __IOCTL_H

#include <bone/api/olaf_api.h>

struct bone_request {
	olaf_code_t code;
	void *arg;
};

#endif