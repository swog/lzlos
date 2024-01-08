#include "interrupts.h"
#include "vga.h"
#include "kdefs.h"
#include "kpic.h"
#include <string.h>

static const char* exception_names[] = {
	"Divide Error",
	"Debug Exception",
	"Nonmaskable external interrupt",
	"Breakpoint",
	"Overflow",
	"Bound Range Exceeded",
	"Invaild Opcode",
	"Device Not Available", 
	"Double Fault", 
	"Co-processor Segment Overrun",
	"Invaild TSS",
	"Segment Not Present",
	"Stack-Segment Fault",
	"General Protection",
	"Page Fault",
	"Reserved",
	"x87 Floating Point Exception",
	"Alignment Check",
	"Machine Check",
	"SIMD Floating-Point Exception",
	"Virtualization Exception",
	"Control Protection Exception",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	
	// Kernel-defined exceptions
};

kinterrupt_t* kernel_interrupts_table[256];

static void print_info(kisrcall_t* info) {
	const char* name = kernel_interrupts_name(info->isr_number);

	if (name) {
		kprintf("CPU interrupt: %s\n", name);
	}	

	kprintf("Interrupt vector: %x\n", info->isr_number);
	kprintf("Error code: %x\n", info->err_code);	
	kprintf("Ss: %x\n", info->ss);
	kprintf("Rsp: %x\n", info->rsp);
	kprintf("Rflags: %x\n", info->rflags);
	kprintf("Cs: %x\n", info->cs);
	kprintf("Rip: %x\n", info->rip);
	kprintf("Rax: %x\n", info->rax);
	kprintf("Rbx: %x\n", info->rbx);
	kprintf("Rcx: %x\n", info->rcx);
	kprintf("Rdx: %x\n", info->rdx);
	kprintf("Rbp: %x\n", info->rbp);
	kprintf("Rdi: %x\n", info->rdi);
	kprintf("Rsi: %x\n", info->rsi);
	kprintf("R8: %x\n", info->r8);
	kprintf("R9: %x\n", info->r9);
	kprintf("R10: %x\n", info->r10);
	kprintf("R11: %x\n", info->r11);
	kprintf("R12: %x\n", info->r12);
	kprintf("R13: %x\n", info->r13);
	kprintf("R14: %x\n", info->r14);
	kprintf("R15: %x\n", info->r15);
}

// Initialize the interrupts table
void kernel_interrupts_main() {
	memset(kernel_interrupts_table, 0, sizeof(kernel_interrupts_table));
}

// Get the name of a CPU interrupt
const char* kernel_interrupts_name(unsigned char num) {
	if (num < ARRAYSIZE(exception_names)) {
		return exception_names[num];
	}
	return NULL;
}

kinterrupt_t* kernel_interrupts_get(unsigned char num) {
	return kernel_interrupts_table[num];
}

void kernel_interrupts_set(unsigned char num, kinterrupt_t* func) {
	kernel_interrupts_table[num] = func;
}

// Handle kernel interrupt vectors
void kernel_isrhandler(kisrcall_t* info) {
	// Halt if there is no interrupt handler, or the handler failed to resume execution.
	kinterrupt_t* func = kernel_interrupts_get(info->isr_number);
	if (func) {
	
		if (func(info) == IRQ_FAILURE) {
			kprintf("Interrupt handler resulted in failure.\n");
			//print_info(info);
			asm("hlt");
		}
		// Interrupt handler success
	}
	else {
		kprintf("Unhandled interrupt.\n");
		print_info(info);
		asm("hlt");
	}

	io_eoi(info->isr_number-IRQ_TIMER);
}
