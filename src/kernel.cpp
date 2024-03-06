
#include <stdio.h>
#include "kdefs.h"
#include "kpic.h"
#include "kbd.h"
#include "multiboot.h"
#include "sys.h"
#include "vga.h"
#include "interrupts.h"
#include "scheduler.h"
#include "elf.h"
#include "libelf.h"
#include "drv.h"

DRIVER_DEFINE(fat);

// PML4
extern size_t page_table_64_l4[512];
extern size_t page_table_64_l3[512];
extern size_t page_table_64_l2[512];

enum {
	page_present = 1,
	page_readwrite,
	page_2mb = 1<<7,	
};

static void kernel_pagetable_init() {
	page_table_64_l4[0] = (size_t)page_table_64_l3 | 0b11;
	page_table_64_l3[0] = (size_t)page_table_64_l2 | 0b11;

	page_table_64_l2[0] = 0x000000 | page_present | page_readwrite | page_2mb;
	// Pages are default executable???
	page_table_64_l2[1] = 0x200000 | page_present | page_readwrite | page_2mb;

	set_cr3(page_table_64_l4);
}

extern "C" void kernel_main() {
	// Remap after hardware NMIs
	pic_remap(0x20, 0x28);
	
	// Mask interrupts that aren't handled
	pic_mask(PIC_MASTER_DATA, ~2);
	// No slave data will be rececived.
	pic_mask(PIC_SLAVE_DATA, ~0);

	// Initialize interrupt handlers
	kernel_interrupts_main();

	// Initialize 64 bit page table
	kernel_pagetable_init();

	// Initialize ksys (libc)
	ksys_main();	
	// Lastly initialize kbd interrupt handler
	kbd_main();
	vga_main();

	// Enable maskable interrupts
	set_interrupts();	

	//scheduler_main(page_table_64_l4);

	void* vbase = NULL;
	DRIVER_MOD(fat)->isize = drv_size(DRIVER_TEB(fat), &vbase);
	DRIVER_MOD(fat)->ibase = (void*)0x200000;

	int r = drv_open(DRIVER_TEB(fat));

	printf("drv_open: %i\n", r);
}












