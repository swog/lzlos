
#include <stdio.h>
#include "kdefs.h"
#include "kpic.h"
#include "kbd.h"
#include "multiboot.h"
#include "mmap.h"
#include "sys.h"
#include "vga.h"

#define IRQ_TIMER 0x20
#define IRQ_KEYBOARD 0x21
#define IRQ_SYSCALL 0x80

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

// Handle all ISRs from the IDT
void kernel_isrhandler(kisrcall_t *info) {
	if (info->isr_number < ARRAYSIZE(exception_names)) {
		vga_puts("Fatal exception: ");
		vga_puts(exception_names[info->isr_number]);
		vga_putc('\n');
		asm("hlt");
	}
	
	if (info->isr_number == IRQ_KEYBOARD) {
		kbd_irq(info);
	}

	if (info->isr_number == IRQ_SYSCALL) {
		vga_puts("SYSCALL\n");
		ksys_irq(info);
	}

	io_eoi(info->isr_number-IRQ_TIMER);
}

void kernel_main(int mbi) {
	// Remap after hardware NMIs
	pic_remap(0x20, 0x28);
	
	// Mask interrupts that aren't handled
	pic_mask(PIC_MASTER_DATA, ~2);
	// No slave data will be rececived.
	pic_mask(PIC_SLAVE_DATA, ~0);

	// Enable maskable interrupts
	set_interrupts();	

	puts("hello crt0!");
}












