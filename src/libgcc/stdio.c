#include <stdio.h>
#include "vga.h"
#include "kdefs.h"
#include "sys.h"

FILE* stdout = (FILE*)STDOUT_FILENO;
FILE* stdin = (FILE*)STDIN_FILENO;

int putchar(int ch) {
	sys_write(STDOUT_FILENO, &ch, 1);

	return ch;
}

int puts(const char* str) {
	sys_write(STDOUT_FILENO, str, strlen(str));
	putchar('\n');

	return 0;
}

int printf(const char* format, ...) {
	return 0;
}
