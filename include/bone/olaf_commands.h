#ifndef __OLAF_COMMANDS_H__
#define __OLAF_COMMANDS_H__

#include <stdint.h>

/* for structures */
#define __packed		__attribute__((packed))

/* code type */
typedef uint64_t olaf_code_t;

/* Olaf command structure:
 *
 * 8 bytes integer
 *
 * +--------+--------+--------+---------+--------+--------+-------+---------+
 * |        |        |        |         |        |        |       |         |
 * |        |        |        |         |        |        |       |         |
 * |        |        |        |         |        |        |       |         |
 * +--------+--------+--------+---------+--------+--------+-------+---------+
 * |	not used for now      |		arg size	          |   id    |
 */

/* Defines new command */
#define OLAF_COMMAND(id, arg_size)	((uint64_t) (((uint8_t) id) | (arg_size << 8)))
#define OLAF_TYPE_COMMAND(id, type)	OLAF_COMMAND(id, sizeof(type))
#define OLAF_COMMAND_SIZE(command)	(command >> 8)

#endif