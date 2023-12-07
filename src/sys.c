#include <stdio.h>
#include "kdefs.h"
#include "sys.h"
#include "vga.h"

// Make sure these don't get linked with libgcc
size_t ksys_read(size_t fd, void* buf, size_t num) {
	return 0;
}

// Kernel system write
size_t ksys_write(size_t fd, const void* buf, size_t num) {
	if (fd == STDOUT_FILENO) {
		const char* src = (const char*)buf;
		
		for (size_t i = 0; i < num; i++) {
			vga_putc(src[i]);
		}
	}

	return num;
}

// System open file
size_t ksys_open(const char* filename, size_t flags, size_t mode) {
	return 0;
}

// System close
size_t ksys_close(size_t fd) {
	return 0;
}

// Handle syscall IRQs
void ksys_irq(kisrcall_t* info) {
	switch (info->rax) {
		case SYS_READ: {
			info->rax = ksys_read(info->rdi, (char*)info->rsi, info->rdx);
			return;
		}
		case SYS_WRITE: {
			info->rax = ksys_write(info->rdi, (const char*)info->rsi, info->rdx);
			return;
		} 
		case SYS_OPEN: {
			info->rax = ksys_open((const char*)info->rdi, info->rsi, info->rdx);
			return;
		}
		case SYS_CLOSE: {
			info->rax = ksys_close(info->rdi);
			return;
		}
	}

	printf("Panic! Unhandled syscall\n");

	// Panic
}
