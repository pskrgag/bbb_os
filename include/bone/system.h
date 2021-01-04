#ifndef __SYSTEM_H
#define __SYSTEM_H

#include <bone/api/olaf_api.h>

/* initial funtions */
extern int bone_init_system(void);
extern void bone_clean_system(void);

extern int bone_disp(olaf_code_t code, void *arg);

#endif