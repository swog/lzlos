#ifndef _SCHEDULER_H
#define _SCHEDULER_H

#include <stddef.h>

typedef struct _lzlos_teb lzlos_teb;

extern "C" lzlos_teb* scheduler_task();

void scheduler_switch(lzlos_teb* teb);

void scheduler_insert(lzlos_teb* teb);

#endif
