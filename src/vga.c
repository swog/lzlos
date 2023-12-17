
#include <stdarg.h>
#include <string.h>
#include "vga.h"
#include "kdefs.h"

#define VGA_WIDTH 80
#define VGA_HEIGHT 25

typedef struct vga_char_s {
    char ch;
    char clr;
} PACKED vga_char_t;

typedef struct vga_info_s {
	int row, col;
	vga_char_t* buf;
	union {
		unsigned char clr;
		struct {
			unsigned char fg : 4;
			unsigned char bg : 4;
		} PACKED;
	};
} vga_info_t;

static vga_info_t vga_info = (vga_info_t) {
	.row = 0,
	.col = 0,
	.buf = (vga_char_t*)0xb8000,
	.fg = VGA_COLOR_WHITE,
	.bg = VGA_COLOR_GREEN
};

static vga_char_t vga_char(char ch) {
   vga_char_t vgc = (vga_char_t) {
        .ch = ch,
        .clr = vga_info.clr
    };

   return vgc;
}

void vga_color(unsigned char fg, unsigned char bg) {
	vga_info.fg = fg;
	vga_info.bg = bg;
}

void vga_clear() {
    vga_char_t vgc = vga_char(' ');
    for (int i = 0; i < VGA_WIDTH*VGA_HEIGHT; i++) {
        vga_info.buf[i] = vgc;
    }
}

void vga_putc(char ch) {
	if (ch == '\n') {
		vga_char_t* vgc = &vga_info.buf[vga_info.col*VGA_WIDTH+vga_info.row];
		vgc->ch = vgc->clr = 0;
		vga_info.row = 0;
	
		if (++vga_info.col >= VGA_HEIGHT) {
			vga_info.col = 0;
		}
	
		return;
	}

 	vga_char_t vgc = vga_char(ch);
	vga_info.buf[vga_info.col*VGA_WIDTH+vga_info.row] = vgc;
	
	if (++vga_info.row >= VGA_WIDTH) {
		vga_info.row = 0;
		if (++vga_info.col >= VGA_HEIGHT) {
			vga_info.col = 0;
		}
	}
}

void vga_puts(const char* str) {
	while (*str) {
		vga_putc(*str);
		str++;
	}
}

#define NIBBLE_HI(x) (((x) & 0xf0) >> 4)
#define NIBBLE_LO(x) ((x) & 0x0f)

// Return- Size of required string
static size_t vga_ztos(size_t num, char* dst, size_t size) {
	// Get required size
	size_t req = 0;
	const char* buf = (const char*)&num;
	
	// Most significant bit
	int msb_reached = 0;

	static const char conv[] = "0123456789abcdef";

	for (size_t i = 0; i < sizeof(size_t); i++) {
		// Null byte before MSB, don't display.
		if (!buf[i] && !msb_reached) {
			continue;
		}
		else if (buf[i]) {
			msb_reached = 1;
		}

		if (dst && size > req+1) {
			dst[req] = conv[NIBBLE_HI(buf[i])];
			dst[++req] = conv[NIBBLE_LO(buf[i])];
		}

		req++;
	}

	if (dist && size > req) {
		dst[req] = '\0';
	}

	return ++req;
}

// Mod- Modifier character
// 1- Failure
// 0- Success
static int vga_format(char mod, char format, va_list ap) {
	switch (format) {
		case 's': {
			vga_puts(va_arg(ap, const char*));
		} break;
		case 'x': {
			const size_t num = va_arg(ap, size_t);
		} break;
		case '%': {
			vga_putc('%');
		} break;
		default:
			return 1;
	}
	return 0;
}

void vga_printf(const char* format, ...) {
	size_t len = strlen(format);

	va_list ap;
	va_start(ap, format);

	for (size_t i = 0; i < len; i++) {
		if (i + 1 < len && format[i] == '%') {
			switch (format[i+1]) {
				case 's': {
					vga_puts(va_arg(ap, const char*));
				} break;
				case 'x': {
					
				} break;
				case '%': {
					vga_putc('%');
				} break;
			}
			i++;
		}
		else {
			vga_putc(format[i]);
		}
	}

	va_end(ap);
}








