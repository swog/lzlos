#include <stdio.h>
#include <string.h>
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
	static const char newline = '\n';

	sys_write(STDOUT_FILENO, str, strlen(str));
	sys_write(STDOUT_FILENO, &newline, 1);

	return 0;
}

int printf(const char* format, ...) {
	sys_write(STDOUT_FILENO, format, strlen(format));
	return 0;
}
