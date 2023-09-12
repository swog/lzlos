
#include "vga.h"
#include "kdefs.h"
#include "kpic.h"
#include "kps2.h"
#include "multiboot.h"
#include "mmap.h"

#define IRQ_TIMER 0x20
#define IRQ_KEYBOARD 0x21

#define INT_PANIC IRQ_TIMER+16

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

int irq_cpl(kisrcall_t *info) {
	return info->cs & 0b11;
}

// Handle panic interrupts
static void panic_irq(kisrcall_t *info) {
	// Only kernel panics
	if (irq_cpl(info) == 0) {
		vga_printf("Panic: %s\n", info->rdi);
		asm("hlt");	
	}
}

// Handle all ISRs from the IDT
void kernel_isrhandler(kisrcall_t *info) {
	if (info->isr_number < sizeof(exception_names)/sizeof(exception_names[0])) {
		vga_printf("Fatal Exception: %s\n", exception_names[info->isr_number]);
		asm("hlt");
	}
	
	if (info->isr_number == IRQ_KEYBOARD) {
		kps2_irq(info);
	}

	if (info->isr_number == INT_PANIC) {
		panic_irq(info);
	}

	io_eoi(info->isr_number-IRQ_TIMER);
}

void kernel_setup_paging() {
}

void kernel_main(int mbi) {
	// Remap after hardware NMIs
	pic_remap(0x20, 0x28);
	
	// Mask interrupts that aren't handled
	pic_mask(PIC_MASTER_DATA, ~2);
	pic_mask(PIC_SLAVE_DATA, ~0);

	// Enable maskable interrupts
	set_interrupts();

	mm_init();

	void *m0 = kalloc(4096);

	vga_printf("%p\n", m0);

	kfree(m0);

	m0 = kalloc(4096);
	void *m1 = kalloc(4096);

	vga_printf("%p,%p\n", m0, m1);

	kfree(m0);
	kfree(m1);
}












