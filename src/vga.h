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
} vga_color;

void vga_clear(unsigned char fg, unsigned char bg);
void vga_putc(char ch, unsigned char fg, unsigned char bg);
void vga_puts_s(const char* str, int len, unsigned char fg, unsigned char bg);
void vga_puts(const char* str, unsigned char fg, unsigned char bg);
void vga_printf(unsigned char fg, unsigned char bg, const char* fmt, ...);