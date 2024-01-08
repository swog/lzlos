#include <stdio.h>
#include "kdefs.h"
#include "sys.h"
#include "vga.h"
#include "interrupts.h"

size_t kfread(void* buf, size_t size, size_t count, FILE* f) {
	return 0;
}

size_t kfwrite(const void* buf, size_t size, size_t count, FILE* f) {
	const char* src = (const char*)buf;	

	if (f == stdout) {
		for (size_t i = 0; i < size*count; i++) {
			kputc(src[i]);
		}

		return size;
	}

	return 0;
}

FILE* kfopen(const char* filename, const char* mode) {
	return NULL;
}

size_t kfclose(FILE* f) {
	return 0;
} 

int kexecve(const char* path, char* const argv[], char* const envp[]) {
	return 0;
}

void ksys_main() {
	kernel_interrupts_set(IRQ_SYSCALL, ksys_irq);
}

// Handle syscall IRQs
// rdi, rsi, rdx, rcx
int ksys_irq(kisrcall_t* info) {
	switch (info->rax) {
		case SYS_READ: {
			return IRQ_SUCCESS;
		}
		case SYS_WRITE: {
			// Hope RCX doesn't interfere with c++... sounds like a fun bug
			info->rax = kfwrite((const void*)info->rdi, info->rsi, info->rdx, (FILE*)info->rcx);
			return IRQ_SUCCESS;
		} 
		case SYS_OPEN: {
			return IRQ_SUCCESS;
		}
		case SYS_CLOSE: {
			return IRQ_SUCCESS;
		}
		case SYS_EXECVE: {
			info->rax = kexecve((const char*)info->rdi, (char* const*)info->rsi, (char* const*)info->rdx);
			return IRQ_SUCCESS;
		}
	}

	// Panic
	return IRQ_FAILURE;
}


