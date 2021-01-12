#ifndef _PD_H
#define _PD_H

#define __END_OF_LIST		{},

#define RDWR			0x11
#define RDONLY			0x01

struct platform_data {
	size_t size;
	const char *serial_number;
	int perm;
};

#endif
