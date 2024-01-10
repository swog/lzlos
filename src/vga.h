#pragma once

typedef enum {
    VGA_COLOR_BLACK = 0,
	VGA_COLOR_BLUE = 1,
	VGA_COLOR_GREEN = 2,
	VGA_COLOR_CYAN = 3,
	VGA_COLOR_RED = 4,
	VGA_COLOR_MAGENTA = 5,
	VGA_COLOR_BROWN = 6,
	VGA_COLOR_LIGHT_GREY = 7,
	VGA_COLOR_DARK_GREY = 8,
	VGA_COLOR_LIGHT_BLUE = 9,
	VGA_COLOR_LIGHT_GREEN = 10,
	VGA_COLOR_LIGHT_CYAN = 11,
	VGA_COLOR_LIGHT_RED = 12,
	VGA_COLOR_LIGHT_MAGENTA = 13,
	VGA_COLOR_LIGHT_BROWN = 14,
	VGA_COLOR_WHITE = 15,
} vga_color_e;

typedef struct kcolor_s {
	unsigned char fg, bg;
} kcolor_t;

void vga_main();
void kcolor(kcolor_t clr);
void kclear();
int kputc(int ch);
int kputs(const char* str);
// This calls stdlib's _vprintf with different function parameters.
// Allows us to only need to use 1 printf function for both kernel & usermode
int kprintf(const char* fmt, ...);
