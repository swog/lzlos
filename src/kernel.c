
#include "vga.h"
#include "ktypes.h"

void* kidtstubs[256];

void kidtstub() {
	vga_puts("Hello, world");
	asm("iret");
}

void kidtfixstub() {
	asm("add 8, %rsp");
	asm("iret");
}

void kpremain() {
	for (int i = 0; i < 256; i++) {
		kidtstubs[i] = kidtstub;
	}
	kidtstubs[0x8] = kidtfixstub;
	kidtstubs[0xA] = kidtfixstub;
	kidtstubs[0xB] = kidtfixstub;
	kidtstubs[0xC] = kidtfixstub;
	kidtstubs[0xD] = kidtfixstub;
	kidtstubs[0xE] = kidtfixstub;
}

void kmain() {
	
}
