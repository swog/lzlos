#include <stdio.h>
#include <string.h>
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
	size_t len = strlen(str);
	size_t ret = 0;

	ret = sys_write(STDOUT_FILENO, str, len);

	if (ret != len) {
		return 1;
	}

	ret = sys_write(STDOUT_FILENO, &newline, 1);

	if (ret != 1) {
		return 2;
	}

	return 0;
}


