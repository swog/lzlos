
#include "vga.h"
#include "kdefs.h"
#include "kpic.h"
#include "kps2.h"
#include "multiboot.h"

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

void kernel_isrhandler(kisrcall_t *info) {
	if (info->isr_number < sizeof(exception_names)/sizeof(exception_names[0])) {
		vga_printf("Fatal Exception: %s\n", exception_names[info->isr_number]);
		asm("hlt");
	}

	
	if (info->isr_number == 0x21) {
		kps2_irq(info);
	}

	io_eoi(info->isr_number-0x20);
}

void kernel_setup_paging() {
}

void kernel_main(int mbi) {
	int magic = *(int*)mbi;
	vga_printf("%p\n", magic); 

	// Remap after hardware NMIs
	pic_remap(0x20, 0x28);
	
	// Mask interrupts that aren't handled
	pic_mask(PIC_MASTER_DATA, ~2);
	pic_mask(PIC_SLAVE_DATA, ~0);

	// Enable maskable interrupts
	set_interrupts();

	
}










