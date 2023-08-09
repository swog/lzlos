
#include <stdarg.h>
#include "vga.h"

#define VGA_WIDTH 80
#define VGA_HEIGHT 25

struct vga_char {
    char ch;
    char col;
};

static struct vga_char vga_char(char ch, unsigned char fg, unsigned char bg) {
    struct vga_char vgc = (struct vga_char) {
        .ch = ch,
        .col = bg << 4 | fg
    };
    return vgc;
}

int vga_row, vga_col;
struct vga_char* vga_buf;

void vga_init() {
	vga_row = vga_col = 0;
	vga_buf = (struct vga_char*)0xb8000;
}

void vga_clear(unsigned char fg, unsigned char bg) {
    struct vga_char vgc = vga_char(' ', fg, bg);
    for (int i = 0; i < VGA_WIDTH*VGA_HEIGHT; i++) {
        vga_buf[i] = vgc;
    }
}

void vga_putc(char ch, unsigned char fg, unsigned char bg) {
	if (ch == '\n') {
		struct vga_char* vgc = &vga_buf[vga_col*VGA_WIDTH+vga_row];
		vgc->ch = vgc->col = 0;
		vga_row = 0;
		if (++vga_col >= VGA_HEIGHT) {
			vga_col = 0;
		}
		return;
	}
    struct vga_char vgc = vga_char(ch, fg, bg);
	vga_buf[vga_col*VGA_WIDTH+vga_row] = vgc;
	if (++vga_row >= VGA_WIDTH) {
		vga_row = 0;
		if (++vga_col >= VGA_HEIGHT) {
			vga_col = 0;
		}
	}
}

void vga_puts_s(const char* str, int len, unsigned char fg, unsigned char bg) {
	for (int i = 0; i < len; i++) {
		vga_putc(str[i], fg, bg);
	}
}

void vga_puts(const char* str, unsigned char fg, unsigned char bg) {
	while (*str) {
		vga_putc(*str, fg, bg);
		str++;
	}
}

static const char hexm[] = "0123456789abcdef";

static void vga_puta(const void* ptr, int size, unsigned char fg, unsigned char bg) {
	char byte;
	for (int i = size-1; i >= 0; i--) {
		byte = ((char*)ptr)[i];
		vga_putc(hexm[(byte & 0xf0) >> 4], fg, bg);
		vga_putc(hexm[byte & 0xf], fg, bg);
	}
}

static void vga_putn(int n, unsigned char fg, unsigned char bg) {
	char byte;
	char front = 1;
	for (int i = sizeof(n)-1; i >= 0; i--) {
		byte = ((char*)&n)[i];
		if (!front) {
			vga_putc(hexm[(byte & 0xf0) >> 4], fg, bg);
			vga_putc(hexm[byte & 0xf], fg, bg);
			continue;
		}
		else if (byte) {
			front = 0;
			// still leading zero
			if ((byte & 0xf0) >> 4) {
				vga_putc(hexm[(byte & 0xf0) >> 4], fg, bg);
			}
			vga_putc(hexm[byte & 0xf], fg, bg);
		}
	}
}

void vga_printf(unsigned char fg, unsigned char bg, const char* format, ...) {
	va_list ap;
	va_start(ap, format);
	while (*format) {
		if (format[0] == '%') {
			switch (format[1]) {
				case 'p': {
					const void* ptr = va_arg(ap, void*);
					vga_puta(&ptr, sizeof(ptr), fg, bg);
				} break;
				case 's': {
					const char* s = va_arg(ap, const char*);
					vga_puts(s, fg, bg);
				} break;
				case 'u': {
					unsigned int num = va_arg(ap, int);
					vga_putn(num, fg, bg);
				} break;
				case 'c': {
					char ch = (char)va_arg(ap, int);
					vga_putc(ch, fg, bg);
				} break;
				case '%': {
					vga_putc('%', fg, bg);
				} break;
			}
			format++;
		}
		else {
			vga_putc(*format, fg, bg);
		}
		format++;
	}
	va_end(ap);
}