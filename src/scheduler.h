#ifndef _SCHEDULER_H
#define _SCHEDULER_H

#include <stddef.h>

typedef struct _lzlos_teb lzlos_teb;

#ifdef __cplusplus
extern "C"
#endif

lzlos_teb* scheduler_task();

void scheduler_switch(lzlos_teb* teb);

#endif
