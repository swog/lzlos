//
// Kernel keyboard module
//

#include "kdefs.h"
#include "vga.h"
#include "kbd.h"
#include "kpic.h"
#include "interrupts.h"

static const char uppercase[] = {
	'\0', '\e', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')',
	'_', '+', '\b', '\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I',
	'O', 'P', '{', '}', '\0', '\0', 'A', 'S', 'D', 'F', 'G', 'H',
	'J', 'K', 'L', ':', '\'', '~', '\0', '\\', 'Z', 'X', 'C', 'V',
	'B', 'N', 'M', '<', '>',  '?', '\0', '\0', '\0', ' '
};

static const char lowercase[] = {
	'\0', '\0', '1', '2', '3', '4', '5', '6',  '7', '8', '9', '0',
	'-', '=', '\b', '\t', 'q', 'w', 'e', 'r',  't', 'y', 'u', 'i',
	'o', 'p', '[', ']', '\0', '\0', 'a', 's',  'd', 'f', 'g', 'h',
	'j', 'k', 'l', ';', '\'', '`', '\0', '\\', 'z', 'x', 'c', 'v',
	'b', 'n', 'm', ',', '.', '/', '\0', '\0', '\0', ' '
};

char kps2_uppercase = 0;
char kps2_caps = 0;

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
	// L shift down
	case 0x2a: {
		kps2_uppercase = 1;
	} break;
	case 0xba: {
		kps2_caps = !kps2_caps;
	} break;
	// backspace
	case 0xe: {
		kputc('\b');
	} break;
	// enter
	case 0x1c: {
		kputc('\n');
	} break;
	default:
		if (key < ARRAYSIZE(lowercase) && key < ARRAYSIZE(uppercase)) {
			const char* keycase = lowercase;
			if (kps2_uppercase || kps2_caps) {
				keycase = uppercase;
			}
			
			if (keycase[key]) {
				kputc(keycase[key]);
			}
		}
		else {
				//kputc('?');
		}
	}

	return IRQ_SUCCESS;
}
