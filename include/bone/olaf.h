#ifndef __OLAF_H__
#define __OLAF_H__

#include <stdint.h>
#include <bone/olaf_api.h>

/* NAME: olaf_call
 * 	Simple wrapper on send() + recv() functions
 * 	Basic function for communicating with olaf
 */
extern uint64_t olaf_call(sock_t socket, olaf_code_t code, void *args, size_t arg_size);

#endif /* __OLAF_H__ */