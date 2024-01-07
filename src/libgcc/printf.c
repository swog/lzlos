#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "sys.h"
#include "kdefs.h"

static void vprintfEx(const char* format, size_t len, size_t* pos, va_list ap) {
	if (*pos >= len) {
		return;
	}	
	else if (format[*pos] != '%') {
		putchar(format[*pos]);
		(*pos)++;
		return;
	}

	switch (format[*pos+1]) {
	case 'i': {
		char s[32];
		int num = va_arg(ap, int);
		size_t len;		

		if (num >= 0) {
			len = ulltostr((unsigned long long)num, 10, s, sizeof(s));
			sys_write(STDOUT_FILENO, s, len);
		}
		else {
			// 2's complement
			num = ~num + 1;
			len = ulltostr((unsigned long long)num, 10, s, sizeof(s));
			putchar('-');
			sys_write(STDOUT_FILENO, s, len);
		}
	
		(*pos)++;
	} break;
	case 'x':
	case 'p': {
		char s[32];
		size_t num = va_arg(ap, size_t);
		size_t len = ulltostr(num, 16, s, sizeof(s));
		sys_write(STDOUT_FILENO, s, len);
		(*pos)++;
	} break;
	case 's': {
		const char* s = va_arg(ap, const char*);
		sys_write(STDOUT_FILENO, s, strlen(s));
		(*pos)++;
	} break;
	default:
		putchar(format[*pos+1]);
		(*pos)++;
	}

	(*pos)++;
}

int vprintf(const char* format, va_list ap) {
	size_t len = strlen(format);
	size_t pos = 0;

	while (pos < len) {
		vprintfEx(format, len, &pos, ap);
	}

	return 0;
}

int printf(const char* format, ...) {
	va_list ap;
	va_start(ap, format);
	int ret = vprintf(format, ap);
	va_end(ap);
	return ret;
}
