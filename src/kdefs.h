#ifndef _KDEFS_H
#define _KDEFS_H

#include <stdint.h>
#include <stddef.h>
#include <memory.h>

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

extern "C" {
	void set_cr3(void *pagetable);
	void set_cr8(void *cr8);

	// Enable maskable interrupts
	void set_interrupts();
}

// LIBC unsigned long to string.
size_t ulltostr(unsigned long long num, int base, char* dst, size_t size); 

#ifndef STDIN_FILENO
#define STDIN_FILENO 0
#endif

#ifndef STDOUT_FILENO
#define STDOUT_FILENO 1
#endif

#ifndef ARRAYSIZE
#define ARRAYSIZE(x) (sizeof(x) / sizeof(x[0]))
#endif

#define TEB_BUSY 1
#define TEB_NAMESIZE 256

// LZLOS Thread Environment Block
typedef struct _lzlos_teb {
	//   Don't change these offsets. They are hard
	// coded into the scheduler_main.asm file
	size_t 	image_size;		// Process image size
	void* 	image_base; 		// Process virtual address
	void*	stack;			// Virtual address
	void*	pagetable;		// Page table linear address
	
	struct _lzlos_teb* prev;
	struct _lzlos_teb* next;
	unsigned char	flags;		// Busy, etc
	unsigned char	priority;	// Higher priority
	size_t		uptime;		// CPU time
	char	name[TEB_NAMESIZE];	// Process name (path), paged
} lzlos_teb;

//   Spec for backwards compatibility:
// 1. Object must be named Name##_driver
// 2. Variable names are finicky to work, keep asm and types.
// 3. Size and Start must be type uint8_t[] and must be named Name##_size, Name##_start respectively.
#define DRIVER_DEFINE(Name) \
	extern uint8_t Name##_size[] asm("_binary_src_incbin_" #Name "_bin_size"); \
	extern uint8_t Name##_start[] asm("_binary_src_incbin_" #Name "_bin_start"); \
	lzlos_teb Name##_driver = {(size_t)Name##_size, Name##_start, NULL, NULL, NULL, NULL, 0, 0, 0, #Name}

// Backwards compatibility with C code (thanks William)
// Spec: Must be a pointer to lzlos_teb*
#define DRIVER_TEB(Name) \
	((lzlos_teb*)(&Name##_driver))

#endif






