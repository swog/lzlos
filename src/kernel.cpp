
#include <stdio.h>
#include "kdefs.h"
#include "kpic.h"
#include "kbd.h"
#include "multiboot.h"
#include "mmap.h"
#include "sys.h"
#include "vga.h"
#include "interrupts.h"
#include "scheduler.h"
#include "elf.h"
#include "libelf.h"

DRIVER_DEFINE(fat);

extern size_t page_table_64_l4[512];
extern size_t page_table_64_l3[512];
extern size_t page_table_64_l2[512];

static void kernel_pagetable_init() {
	page_table_64_l4[0] = (size_t)page_table_64_l3 | 0b11;
	page_table_64_l3[0] = (size_t)page_table_64_l2 | 0b11;

	page_table_64_l2[0] = 0b10000011;
	page_table_64_l2[1] = 0x200000 | 0b10000011;

	set_cr3(page_table_64_l4);
}

EXTERN_C void kernel_main() {
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

	// Set 64 bit page table.
	set_cr3(page_table_64_l4);

	scheduler_main(page_table_64_l4);
}












