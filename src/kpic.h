#pragma once

#define PIC_MASTER_CMD 0x20
#define PIC_MASTER_DATA 0x21
#define PIC_SLAVE_CMD 0xA0
#define PIC_SLAVE_DATA 0xA1
#define PIC_EOI 0x20

// Output byte to IO port
extern "C" void io_outb(int port, int byte);

// Input byte from IO port.
extern "C" int io_inb(int port);

// Turned into macro since it would just call outb.
#define io_wait() io_outb(0x80, 0)

// End of instruction to master. Slave is notified if the irq is greater than 7. 
void io_eoi(int irq);

// Set the PIC's maskable interrupt's interrupt vector bases
// These are set to 0x20, the first user-defined interrupt vectors
void pic_remap(int master, int slave);

// Set the mask for PIC interrupts.
// If a bit is set, the PIC ignores the IRQ.
void pic_mask(int data_port, int mask);

#define pic_mask_read io_inb
