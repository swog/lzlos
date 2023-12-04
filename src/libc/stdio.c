#include <stdio.h>
#include "vga.h"
#include "kdefs.h"

EXPORT int putchar(int ch) {
	// Send to vga for now.
	vga_putc(ch);

	return ch;
}

EXPORT int puts(const char* str) {
	vga_puts(str);
	vga_putc('\n');

	return 0;
}

EXPORT int printf(const char* format, ...) {
	

	return 0;
}
