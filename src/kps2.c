//
// Kernel keyboard module
//

#include "kdefs.h"
#include "vga.h"

static char keys[] = {
	'\0', '\0', '1', '2', '3', '4', '5', '6',  '7', '8', '9', '0',
	'-', '=', '\b', '\t', 'q', 'w', 'e', 'r',  't', 'y', 'u', 'i',
	'o', 'p', '[', ']', '\0', '\0', 'a', 's',  'd', 'f', 'g', 'h',
	'j', 'k', 'l', ';', '\'', '`', '\0', '\\', 'z', 'x', 'c', 'v',
	'b', 'n', 'm', ',', '.', '/', '\0', '\0', '\0', ' '
};

void kps2_irq(kisrcall_t *info) {
	int key = io_inb(0x60);

	if (key <= 128) {
		vga_putc(keys[key]);
	}
}
