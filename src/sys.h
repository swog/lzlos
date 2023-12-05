#ifndef _SYS_H
#define _SYS_H

// RAX values
#define SYS_READ 	0
#define SYS_WRITE 	1
#define SYS_OPEN 	2
#define SYS_CLOSE 	3

#include <stdio.h>

size_t sys_read(int fd, const void* buf, size_t size);
size_t sys_write(int fd, const void* buf, size_t size);

typedef struct kisrcall_s kisrcall_t;

// Handle syscall IRQs
void ksys_irq(kisrcall_t* info);

#endif
