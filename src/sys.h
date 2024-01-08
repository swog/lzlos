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

#define SYS_EXECVE	59

#include <stdio.h>

// Kernel counterparts
size_t kfread(void* buf, size_t size, size_t count, FILE* f);
size_t kfwrite(const void* buf, size_t size, size_t count, FILE* f);
FILE* kfopen(const char* filename, const char* mode);
size_t kfclose(FILE* f);

// ISR call stack kdefs
typedef struct kisrcall_s kisrcall_t;

// Handle syscall IRQs
int ksys_irq(kisrcall_t* info);

void ksys_main();

#endif
