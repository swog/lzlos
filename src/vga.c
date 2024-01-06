
#include <stdarg.h>
#include <string.h>
#include "vga.h"
#include "kdefs.h"
#include <stdlib.h>

#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_CHAR(bg, fg, ch) ((ch) | ((bg << 12) | (fg << 8)))

unsigned char vga_row = 0, vga_col = 0;
uint16_t* vga_history = (uint16_t*)0xb8000;
// Up & down backbuffers.
uint16_t vga_up_backbuffer[VGA_HEIGHT][VGA_WIDTH];
uint16_t vga_dn_backbuffer[VGA_HEIGHT][VGA_WIDTH];

void vga_init() {
	memset(vga_up_backbuffer, 0, sizeof(vga_up_backbuffer));
	memset(vga_dn_backbuffer, 0, sizeof(vga_dn_backbuffer));
}

void kputc(char ch) {
	if (ch == '\n') {
		vga_col = 0;
		if (++vga_row >= VGA_HEIGHT) {
			vga_row = 0;
		}
		return;
	}

	vga_history[vga_row*VGA_WIDTH+vga_col] = VGA_CHAR(VGA_COLOR_BLACK, VGA_COLOR_GREEN, ch);

	if (++vga_col >= VGA_WIDTH) {
		vga_col = 0;
		if (++vga_row >= VGA_HEIGHT) {
			vga_row = 0;
		}
	}
}

static void kputhex(const void* data, size_t size) {
	const unsigned char* bytes = (const unsigned char*)data;
	static const char hb[] = "0123456789abcdef";

	for (int i = size-1; i >= 0; i--) {
		kputc(hb[(bytes[i] & 0xf0) >> 4]);
		kputc(hb[(bytes[i] & 0x0f)]);
	}
}

void kprintf(const char* format, ...) {
	va_list ap;
	va_start(ap, format);

	size_t len = strlen(format);

	for (size_t i = 0; i < len; i++) {
		if (format[i] != '%' || i+1 >= len) {
			kputc(format[i]);
			continue;
		}	

		switch (format[i+1]) {
			case '%': {
				kputc('%');
			} break;
			case 'x':
			case 'p': {
				size_t p = va_arg(ap, size_t);
				kputhex(&p, sizeof(p));
			} break;
			case 's': {
				const char* s = va_arg(ap, const char*);
				while (*s) {
					kputc(*s);
					s++;
				}
			} break;
		}
	
		i++;
	}

	va_end(ap);
}







