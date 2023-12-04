#ifndef _KDEFS_H
#define _KDEFS_H

typedef char i8;
typedef unsigned char u8;
typedef short i16;
typedef unsigned short u16;
typedef long i32;
typedef unsigned long u32;
typedef long long i64;
typedef unsigned long long u64;

#define EXPORT __attribute__((visibility("default")))
#define PACKED __attribute__((packed))
#define ALIGNED(x) __attribute__((aligned(x)))

// Interrupt service routine's stack
typedef struct kisrcall_s {
	u64 r15;
	u64 r14;
	u64 r13;
	u64 r12;
	u64 r11;
	u64 r10;
	u64 r9;
	u64 r8;
	u64 rsi;
	u64 rdi;
	u64 rbp;
	u64 rdx;
	u64 rcx;
	u64 rbx;
	u64 rax;
	u64 isr_number;
	u64 err_code;
	u64 rip;
	u64 cs; 
	u64 rflags; 
	u64 rsp;
	u64 ss;
} kisrcall_t;

typedef void kisr_handler(kisrcall_t *info);

void set_cr3(void *pagetable);
void set_cr8(void *cr8);

// Enable maskable interrupts
void set_interrupts();

// Get current process level
int get_cpl();

// Hault the machine
void panic(const char *str);

#ifndef ARRAYSIZE
#define ARRAYSIZE(x) (sizeof(x) / sizeof(x[0]))
#endif

#ifndef NULL
#define NULL ((void*)0)
#endif

#endif
