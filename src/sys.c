#include <stdio.h>
#include "kdefs.h"
#include "sys.h"
#include "vga.h"
#include "interrupts.h"

// Make sure these don't get linked with libgcc
int ksys_read(kisrcall_t* info) {
	return IRQ_SUCCESS;
}

// Kernel system write
int ksys_write(kisrcall_t* info) {
	if (info->rdi == STDOUT_FILENO) {
		const char* src = (const char*)info->rsi;
		
		for (size_t i = 0; i < num; i++) {
			kputc(src[i]);
		}
	}

	return IRQ_SUCCESS;
}

// System open file
int ksys_open(kisrcall_t* info) {
	return IRQ_SUCCESS;
}

// System close
int ksys_close(kisrcall_t* info) {
	return IRQ_SUCCESS;
}

void ksys_main() {
	kernel_interrupts_set(IRQ_SYSCALL, ksys_irq);
}

// Handle syscall IRQs
int ksys_irq(kisrcall_t* info) {
	switch (info->rax) {
		case SYS_READ: {
			return ksys_read(info);
		}
		case SYS_WRITE: {
			return ksys_write(info);
		} 
		case SYS_OPEN: {
			return ksys_open(info);
		}
		case SYS_CLOSE: {
			return ksys_close(info);
		}
	}

	// Panic
	return IRQ_FAILURE;
}


