#include <stdio.h>
#include "kdefs.h"
#include "sys.h"
#include "vga.h"
#include "interrupts.h"

size_t sys_read(size_t fd, void* buf, size_t size);

size_t ksys_write(size_t fd, const void* buf, size_t size) {
	const char* src = (const char*)buf;	

	if (fd == STDOUT_FILENO) {
		for (size_t i = 0; i < size; i++) {
			kputc(src[i]);
		}

		return size;
	}

	return 0;
}

size_t sys_open(const char* filename, size_t flags, size_t mode);
size_t sys_close(size_t fd); 

void ksys_main() {
	kernel_interrupts_set(IRQ_SYSCALL, ksys_irq);
}

// Handle syscall IRQs
int ksys_irq(kisrcall_t* info) {
	switch (info->rax) {
		case SYS_READ: {
			return IRQ_SUCCESS;
		}
		case SYS_WRITE: {
			info->rax = ksys_write(info->rdi, (const void*)info->rsi, info->rdx);
			return IRQ_SUCCESS;
		} 
		case SYS_OPEN: {
			return IRQ_SUCCESS;
		}
		case SYS_CLOSE: {
			return IRQ_SUCCESS;
		}
	}

	// Panic
	return IRQ_FAILURE;
}


