#ifndef _SCHEDULER_H
#define _SCHEDULER_H

#include <stddef.h>
#include "kdefs.h"

// SYS_EXECVE 0x59
int kexecve(const char* path, char* const argv[], char* const envp[]);

// SYS_EXIT 0x60
void kexit(int status);

#endif
