#ifndef _SCHEDULER_H
#define _SCHEDULER_H

#include <stddef.h>
#include "kdefs.h"

// SYS_EXECVE 0x59
int kexecve(const char* path, char* const argv[], char* const envp[]);

// SYS_EXIT 0x60
void kexit(int status);

// TEB forward decl
typedef struct _lzlos_teb lzlos_teb;

// Current task
EXTERN_C lzlos_teb* scheduler_task();

// Task switch
EXTERN_C void scheduler_switch(lzlos_teb* teb);

void scheduler_insert(lzlos_teb* teb);
void scheduler_remove(lzlos_teb* teb);

void scheduler_main(const void* pagetable);

#endif
