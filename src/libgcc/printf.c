#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "sys.h"
#include "kdefs.h"
#include "lzlos.h"

int _vprintf(const char* format, va_list ap, putchar_t putchar, fwrite_t fwrite) {
	size_t len = strlen(format);

	for (size_t i = 0; i < len; i++) {
		if (format[i] != '%' || i+1 >= len || format[i+1] == '%') {
			putchar(format[i]);
			continue;
		}

		switch (format[i+1]) {
		case 'c': {
			int c = va_arg(ap, int);
			putchar(c);
			i++;
		} break;
		case 'l':
		case 'i': {
			char buf[32];
			int num = va_arg(ap, int);
			if (num < 0) {
				num = ~num + 1;
				putchar('-');
			}
			size_t len = ulltostr((unsigned long long)num, 10, NULL, 0);
			if (len < sizeof(buf)) {
				ulltostr((unsigned long long)num, 10, buf, len);
				fwrite(buf, len, 1, stdout);
			}
			i++;
		} break;
		case 'u':
		case 'x': {
			char buf[32];
			size_t num = va_arg(ap, size_t);
			size_t len = ulltostr((unsigned long long)num, 16, NULL, 0);
			if (len < sizeof(buf)) {
				ulltostr((unsigned long long)num, 16, buf, len);
				fwrite(buf, len, 1, stdout);
			}
			i++;
		} break;
		case 'p': {
			char buf[32];
			size_t num = va_arg(ap, size_t);
			ulltostr((unsigned long long)num, 16, buf, 17);
			fwrite(buf, 8, 1, stdout);
			putchar('`');
			fwrite(buf+8, 8, 1, stdout);
			i++;
		} break;
		case 's': {
			const char* s = va_arg(ap, const char*);
			fwrite(s, strlen(s), 1, stdout);
			i++;
		} break;
		default:
			fwrite(format+i, 2, 1, stdout);
			i++;
		}
	}

	return 0;
}

int vprintf(const char* format, va_list ap) {
	return _vprintf(format, ap, putchar, fwrite);
}

int printf(const char* format, ...) {
	va_list ap;
	va_start(ap, format);
	int ret = vprintf(format, ap);
	va_end(ap);
	return ret;
}
