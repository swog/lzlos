#ifndef _KDEFS_H
#define _KDEFS_H

#include <stdint.h>

#define EXPORT __attribute__((visibility("default")))
#define PACKED __attribute__((packed))
#define ALIGNED(x) __attribute__((aligned(x)))

// Interrupt service routine's stack
typedef struct kisrcall_s {
	uint64_t r15;
	uint64_t r14;
	uint64_t r13;
	uint64_t r12;
	uint64_t r11;
	uint64_t r10;
	uint64_t r9;
	uint64_t r8;
	uint64_t rsi;
	uint64_t rdi;
	uint64_t rbp;
	uint64_t rdx;
	uint64_t rcx;
	uint64_t rbx;
	uint64_t rax;
	uint64_t isr_number;
	uint64_t err_code;
	uint64_t rip;
	uint64_t cs; 
	uint64_t rflags; 
	uint64_t rsp;
	uint64_t ss;
} kisrcall_t;

void set_cr3(void *pagetable);
void set_cr8(void *cr8);

// Enable maskable interrupts
void set_interrupts();

#ifndef STDIN_FILENO
#define STDIN_FILENO 0
#endif

#ifndef STDOUT_FILENO
#define STDOUT_FILENO 1
#endif

#ifndef ARRAYSIZE
#define ARRAYSIZE(x) (sizeof(x) / sizeof(x[0]))
#endif

#endif
