#include "interrupts.h"
#include "vga.h"
#include "kdefs.h"
#include "kpic.h"
#include "libgcc/sys.h"
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

kinterrupt_t kernel_interrupts_table[256];

extern void* isr_handler;

static void print_info(kisrcall_t* info) {
	const char* name = kernel_interrupts_name(info->isr_number);

	if (name) {
		vga_printf("CPU interrupt: %s\n", name);
	}	

	vga_printf("Interrupt vector: %x\n", info->isr_number);
	vga_printf("Error code: %x\n", info->err_code);
	
	if (info->err_code & 1) {
		vga_printf("-External event error.\n");
	}

	if (info->err_code & 2) {
		vga_printf("-IDT descriptor error.\n");
	}

	if (info->err_code & 4) {
		vga_printf("-GDT/LDT descriptor error.\n");
	}	

	vga_printf("Ss: %x\n", info->ss);
	vga_printf("Rsp: %x\n", info->rsp);
	vga_printf("Rflags: %x\n", info->rflags);
	vga_printf("Cs: %x\n", info->cs);
	vga_printf("Rip: %x\n", info->rip);
	vga_printf("Rax: %x\n", info->rax);
	vga_printf("Rbx: %x\n", info->rbx);
	vga_printf("Rcx: %x\n", info->rcx);
	vga_printf("Rdx: %x\n", info->rdx);
	vga_printf("Rbp: %x\n", info->rbp);
	vga_printf("Rdi: %x\n", info->rdi);
	vga_printf("Rsi: %x\n", info->rsi);
	vga_printf("R8: %x\n", info->r8);
	vga_printf("R9: %x\n", info->r9);
	vga_printf("R10: %x\n", info->r10);
	vga_printf("R11: %x\n", info->r11);
	vga_printf("R12: %x\n", info->r12);
	vga_printf("R13: %x\n", info->r13);
	vga_printf("R14: %x\n", info->r14);
	vga_printf("R15: %x\n", info->r15);
}

// General Protection Fault
// Typically an invalid instruction
static int kernel_interrupts_gpf(kisrcall_t* info) {
	
	return IRQ_FAILURE;
}

// Initialize the interrupts table
void kernel_interrupts_main() {
	memset(kernel_interrupts_table, 0, sizeof(kernel_interrupts_table));
	kernel_interrupts_table[0xd] = kernel_interrupts_gpf;
}

// Get the name of a CPU interrupt
const char* kernel_interrupts_name(unsigned char num) {
	if (num < ARRAYSIZE(exception_names)) {
		return exception_names[num];
	}
	return NULL;
}

kinterrupt_t kernel_interrupts_get(unsigned char num) {
	return kernel_interrupts_table[num];
}

void kernel_interrupts_set(unsigned char num, kinterrupt_t func) {
	kernel_interrupts_table[num] = func;
}

// Handle kernel interrupt vectors
void kernel_isrhandler(kisrcall_t* info) {
	// Halt if there is no interrupt handler, or the handler failed to resume execution.
	kinterrupt_t func = kernel_interrupts_get(info->isr_number);
	if (func) {
		if (func(info) == IRQ_FAILURE) {
			vga_printf("Fatal ISR.\n");
			print_info(info);
			asm("hlt");
		}
		// Interrupt handler success
	}
	else {
		vga_printf("Unhandled interrupt.\n");
		print_info(info);
		asm("hlt");
	}

	io_eoi(info->isr_number-IRQ_TIMER);
}
