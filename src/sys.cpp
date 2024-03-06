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

// SYS_EXECVE 0x59
int kexecve(const char* path, char* const argv[], char* const envp[]);

// SYS_EXIT 0x60
void kexit(int status);

void ksys_main() {
	kernel_interrupts_set(IRQ_SYSCALL, ksys_irq);
}

// Handle syscall IRQs
// rdi, rsi, rdx, rcx
int ksys_irq(kisrcall_t* info) {
	switch (info->rax) {
	case SYS_READ: {
	} break;
	case SYS_WRITE: {
		// Hope RCX doesn't interfere with c++... sounds like a fun bug
		info->rax = kfwrite((const void*)info->rdi, info->rsi, info->rdx, (FILE*)info->rcx);
	} break;
	case SYS_OPEN: {
	} break;
	case SYS_CLOSE: {
	} break;
	case SYS_EXECVE: {
		info->rax = kexecve((const char*)info->rdi, (char* const*)info->rsi, (char* const*)info->rdx);
	} break;
	case SYS_EXIT: {
		kexit((int)info->rdi);
	} break;
	case SYS_MMAP: {
		//info->rax = (size_t)kmmap((void*)info->rdi, info->rsi, (int)info->rdx, (int)info->rcx);
	} break;
	case SYS_MUNMAP: {
		//info->rax = (int)kmunmap((void*)info->rdi, info->rsi);
	} break;
	default:
		return IRQ_FAILURE;
	}

	return IRQ_SUCCESS;
}


