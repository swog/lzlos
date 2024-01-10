
#include <stdio.h>
#include "kdefs.h"
#include "kpic.h"
#include "kbd.h"
#include "multiboot.h"
#include "mmap.h"
#include "sys.h"
#include "vga.h"
#include "interrupts.h"

extern uint8_t test_bin[] asm("_binary_src_incbin_test_bin_start");
extern uint8_t test_size[] asm("_binary_src_incbin_test_bin_size");

void kernel_main(int mbi) {
	// Remap after hardware NMIs
	pic_remap(0x20, 0x28);
	
	// Mask interrupts that aren't handled
	pic_mask(PIC_MASTER_DATA, ~2);
	// No slave data will be rececived.
	pic_mask(PIC_SLAVE_DATA, ~0);

	// Initialize interrupt handlers
	kernel_interrupts_main();
	// Initialize ksys (libc)
	ksys_main();	
	// Lastly initialize kbd interrupt handler
	kbd_main();
	vga_main();

	// Enable maskable interrupts
	set_interrupts();	

	for (size_t i = 0; i < (size_t)test_size; i++) {
		kprintf("%c", test_bin[i]);
	}

	//vga_putc('a');
}












