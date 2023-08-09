
#include "vga.h"

void vga_init();

// The kernel will have already cleared the terminal

void kernel_main() {
	vga_init();
	vga_printf(VGA_COLOR_WHITE, VGA_COLOR_GREEN, "hello, %s", "world");
}
