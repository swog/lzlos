#ifndef _LIBC_SYS_H
#define _LIBC_SYS_H

#include <stdint.h>

// Shared counterparts
size_t sys_read(size_t fd, void* buf, size_t size);
size_t sys_write(size_t fd, const void* buf, size_t size);
size_t sys_open(const char* filename, size_t flags, size_t mode);
size_t sys_close(size_t fd); 

#endif
