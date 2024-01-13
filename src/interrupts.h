#ifndef _INTERRUPTS_H
#define _INTERRUPTS_H

typedef struct kisrcall_s kisrcall_t;

#define IRQ_TIMER 	0x20
#define IRQ_KEYBOARD 	0x21
#define IRQ_SYSCALL 	0x80

#define IRQ_FAILURE 	0
#define IRQ_SUCCESS 	1

// Return 0 on failure to return execution; halt.
// Return 1 to continue execution.
typedef int kinterrupt_t(kisrcall_t* info);

// Main interrupt handler called from idt.asm
extern "C" void kernel_isrhandler(kisrcall_t* info);

// Called in kernel_main
// Initializes interrupt table
void kernel_interrupts_main();

// Set callback
void kernel_interrupts_set(unsigned char index,
	kinterrupt_t* callback);

// Interrupts table
extern kinterrupt_t* kernel_interrupts_table[256];

const char* kernel_interrupts_name(unsigned char num);

kinterrupt_t* kernel_interrupts_get(unsigned char num);

#endif
