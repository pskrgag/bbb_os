#ifndef __OLAF_API_H__
#define __OLAF_API_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <bone/api/olaf_api.h>

/* NAME: olaf_call
 * 	Simple wrapper on send() + recv() functions
 * 	Basic function for communicating with olaf
 */
extern ssize_t olaf_call(sock_t socket, olaf_code_t code, void *args);

#ifdef __cplusplus
}
#endif

#endif /* __OLAF_API_H__ */