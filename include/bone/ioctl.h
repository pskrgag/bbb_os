#ifndef __IOCLT_H
#define __IOCTL_H

#include <bone/api/olaf_api.h>
#include <linux/ioctl.h>

struct bone_request {
	olaf_code_t code;
#ifdef __KERNEL__
	void __user *arg;
#else
	void *arg;
#endif
};

#define BONE_DEV	_IOW('D', 0, struct bone_request)

#endif