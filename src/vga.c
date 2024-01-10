
#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "vga.h"
#include "kdefs.h"
#include "sys.h"
#include "libgcc/lzlos.h"

#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_CHAR(bg, fg, ch) ((ch) | ((bg << 12) | (fg << 8)))

unsigned char vga_row = 0, vga_col = 0;
uint16_t* vga_history = (uint16_t*)0xb8000;
// Up & down backbuffers.
uint16_t vga_backbuffer[VGA_HEIGHT][VGA_WIDTH];

void vga_main() {
	memset(vga_backbuffer, 0, sizeof(vga_backbuffer));
}

// Refactor set
void vga_setw(int row, int col, uint16_t c) {
	vga_history[row*VGA_WIDTH+col] = c;
}

// Increment column
// Expected to increment row if column overflow
void vga_incw() {
	if (++vga_col >= VGA_WIDTH) {
		vga_col = 0;
		if (++vga_row >= VGA_HEIGHT) {
			vga_row = 0;
		}
	}
}

// Refactor get
uint16_t vga_getw(int row, int col) {
	return vga_history[row*VGA_WIDTH+col];
}

// Hacky with libgcc.
// Temporary, libc uses some character trait tables or something..
int _isblank(int ch);

// Decrement column
// Expected to decrement row if column overflow
void vga_decw() {
	if (vga_col-- == 0) {
		if (vga_row-- == 0) {
			vga_row = 0;
		}

		vga_col = VGA_WIDTH-1;

		// Rewind while column > 0 and getw is black
		while (vga_col > 0) {
			char c = 0xff & vga_getw(vga_row, vga_col);

			if (c == '\n') {
				break;
			}

			vga_col--;
		}

		for (int y = vga_row+1; y < VGA_HEIGHT-1; y++) {
			memcpy(&vga_history[y*VGA_WIDTH], &vga_history[(y+1)*VGA_WIDTH], VGA_WIDTH*2);
		}

		memset(&vga_history[(VGA_HEIGHT-1)*VGA_WIDTH], 0, VGA_WIDTH*2);
	}
}

// Increment row (height)
void vga_inch() {
	vga_col = 0;
	if (++vga_row >= VGA_HEIGHT) {
		vga_row = 0;
	}
}

int kputc(int ch) {
	switch (ch) {
	case '\n': {
		// Put newline before EOL
		vga_setw(vga_row, vga_col, VGA_CHAR(VGA_COLOR_BLACK, VGA_COLOR_BLACK, ch));
		// Increment height
		vga_inch();
	} break;
	// Ignore
	case '\r': {
	} break;
	case '\t': {
		kputc(' ');
		kputc(' ');
	} break;
	case '\b': {
		vga_decw();
		// Replace with empty space
		kputc(' ');
		vga_decw();
	} break;
	default:
		vga_setw(vga_row, vga_col, VGA_CHAR(VGA_COLOR_BLACK, VGA_COLOR_GREEN, ch));
		vga_incw();
	}

	return 0;
}

int kputs(const char* s) {
	while (*s) {
		kputc(*s);
		s++;
	}
	// Spec...
	kputc('\n');
	return 0;
}

int kprintf(const char* format, ...) {
	va_list ap;
	va_start(ap, format);
	_vprintf(format, ap, kputc, kfwrite);
	va_end(ap);
	return 0;
}








