//
// System library
// 
// Stdlib implementing system calls
// File system interface
// 

#ifndef _SYS_H
#define _SYS_H

// RAX values
#define SYS_READ 	0
#define SYS_WRITE 	1
#define SYS_OPEN 	2
#define SYS_CLOSE 	3

#include <stdio.h>

// Kernel counterparts
size_t ksys_read(size_t fd, void* buf, size_t size);
size_t ksys_write(size_t fd, const void* buf, size_t size);
size_t ksys_open(const char* filename, size_t flags, size_t size);
size_t ksys_close(size_t fd);

// ISR call stack kdefs
typedef struct kisrcall_s kisrcall_t;

// Handle syscall IRQs
void ksys_irq(kisrcall_t* info);

#endif
