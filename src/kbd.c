//
// Kernel keyboard module
//

#include "kdefs.h"
#include "vga.h"
//#include "cbuf.h"
#include "kpic.h"
#include "kbd.h"
#include "interrupts.h"

static char uppercase[] = {
	'\0', '\e', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')',
	'_', '+', '\b', '\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I',
	'O', 'P', '{', '}', '\0', '\0', 'A', 'S', 'D', 'F', 'G', 'H',
	'J', 'K', 'L', ':', '\'', '~', '\0', '\\', 'Z', 'X', 'C', 'V',
	'B', 'N', 'M', '<', '>',  '?', '\0', '\0', '\0', ' '
};

static char lowercase[] = {
	'\0', '\0', '1', '2', '3', '4', '5', '6',  '7', '8', '9', '0',
	'-', '=', '\b', '\t', 'q', 'w', 'e', 'r',  't', 'y', 'u', 'i',
	'o', 'p', '[', ']', '\0', '\0', 'a', 's',  'd', 'f', 'g', 'h',
	'j', 'k', 'l', ';', '\'', '`', '\0', '\\', 'z', 'x', 'c', 'v',
	'b', 'n', 'm', ',', '.', '/', '\0', '\0', '\0', ' '
};

int kps2_uppercase = 0;

// Add the interrupt handler
void kbd_main() {
	kernel_interrupts_set(IRQ_KEYBOARD, kbd_irq);
}

int kbd_irq(kisrcall_t *info) {
	int key = io_inb(0x60);

	switch (key) {
		// L shift release
		case 0xaa: {
			kps2_uppercase = 0;
		} break;
		case 0x2a: {
			kps2_uppercase = 1;
		} break;
		// backspace
		case 0xe: {
			vga_putc('\b');
		} break;
		// enter
		case 0x1c: {
			vga_putc('\n');
		} break;
		default:
			if (key <= 128) {
				if (kps2_uppercase) {
					vga_putc(uppercase[key]);
				}
				else {
					vga_putc(lowercase[key]);
				}
			}
			else {
				//vga_putc('?');
			}
	}
}
