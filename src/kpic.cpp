#include "kpic.h"
#include "kdefs.h"

// Set the PIC's maskable interrupt's interrupt vector bases
// These are set to 0x20, the first user-defined interrupt vectors
void pic_remap(int master, int slave) {
	io_outb(PIC_MASTER_CMD, 0x10 + 0x01);
	io_outb(PIC_SLAVE_CMD,  0x10 + 0x01);
	
	/* Setup master's vector offset */
	io_outb(PIC_MASTER_DATA, master);
	
	/* Tell the slave its vector offset */
	io_outb(PIC_SLAVE_DATA, slave);
	
	/* Tell the master that he has a slave */
	io_outb(PIC_MASTER_DATA, 4);
	io_outb(PIC_SLAVE_DATA, 2);
	
	/* Enabled 8086 mode */
	io_outb(PIC_MASTER_DATA, 0x01);
	io_outb(PIC_SLAVE_DATA, 0x01);

	io_outb(PIC_MASTER_DATA, 0);
	io_outb(PIC_SLAVE_DATA, 0);	

	set_cr8(0);
}

// Set the mask for PIC interrupts.
// If a bit is set, the PIC ignores the IRQ.
void pic_mask(int data_port, int mask) {
	io_outb(data_port, mask);
}

// End of instruction to master. Slave is notified if the irq is greater than 7. 
void io_eoi(int irq) {
	if (irq > 7) {
		io_outb(PIC_SLAVE_CMD, PIC_EOI);
	}
	io_outb(PIC_MASTER_CMD, PIC_EOI);
}
