#ifndef __OLAF_COMMANDS_H
#define __OLAF_COMMANDS_H

#include <stddef.h>

/* for structures */
#ifndef __packed
#define __packed		__attribute__((packed))
#endif

/* code type */
typedef uint64_t olaf_code_t;

struct olaf_packet {
	olaf_code_t code;
}__packed;

/* Olaf command structure:
 *
 * 8 bytes integer
 *
 * +--------+--------+--------+---------+--------+--------+-------+---------+
 * |        |        |        |         |        |        |       |         |
 * |        |        |        |         |        |        |       |         |
 * |        |        |        |         |        |        |       |         |
 * +--------+--------+--------+---------+--------+--------+-------+---------+
 * |     not used    |	perms |           arg size                |   id    |
 * 
 * Read commads allows fill input structure with device info
 * Write commands allows send structure to device
 */

#define OLAF_READ				(1 << 0)
#define OLAF_WRITE				(1 << 1)

/* Defines new command */
#define __OLAF_COMMAND(id, arg_size, perms)	((uint64_t) (id | (arg_size << 8) | ( (uint64_t) perms << 40)))
#define OLAF_COMMAND(id, type, perms)		__OLAF_COMMAND(id, sizeof(type), perms)

#define OLAF_COMMAND_ARG_SIZE(command)		((command >> 8) & 0xffffffff)
#define OLAF_COMMAND_PERMS(command)		(command >> 40)

#define OLAF_WRONG_CODE				(~0ull)

#endif /* __OLAF_COMMANDS_H__ */
