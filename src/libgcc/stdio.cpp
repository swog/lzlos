#include <stdio.h>
#include <string.h>
#include "kdefs.h"
#include "sys.h"

FILE* stdout = (FILE*)STDOUT_FILENO;
FILE* stdin = (FILE*)STDIN_FILENO;

int putchar(int ch) {
	fwrite(&ch, 1, 1, stdout);

	return ch;
}

int puts(const char* str) {
	static const char newline = '\n';
	size_t len = strlen(str);
	size_t ret = 0;

	ret = fwrite(str, len, 1, stdout);

	if (ret != len) {
		return 1;
	}

	ret = fwrite(&newline, 1, 1, stdout);

	if (ret != 1) {
		return 2;
	}

	return 0;
}


