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

#define SYS_MMAP	9
#define SYS_MUNMAP	11

#define SYS_EXECVE	59
#define SYS_EXIT	60

#include <stdio.h>

// Kernel counterparts
size_t 	kfread(void* buf, size_t size, size_t count, FILE* f);
size_t 	kfwrite(const void* buf, size_t size, size_t count, FILE* f);
FILE* 	kfopen(const char* filename, const char* mode);
size_t 	kfclose(FILE* f);

// Align 1kb
// This is used for page tables
#define MMAP_FLAGS_A1KB (1<<0)

// Map
void*	kmmap(void* addr, size_t len, int prot, int flags);
int	kmunmap(void* ptr, size_t size);

// ISR call stack kdefs
typedef struct kisrcall_s kisrcall_t;

// Handle syscall IRQs
int ksys_irq(kisrcall_t* info);

void ksys_main();

#endif
