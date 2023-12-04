
#include <stdarg.h>
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


