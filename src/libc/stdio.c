#include <stdio.h>
#include "vga.h"

int putchar(int ch) {
	// Send to vga for now.
	vga_putc(ch);

	return ch;
}

int printf(const char* format, ...) {
	

	return 0;
}
