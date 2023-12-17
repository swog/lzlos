#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char* ulltostr(unsigned long long num, char* buf, size_t size, size_t base) {
	if (size >= 1) {
		buf[0] = '\0';
	}

	const char* table = "0123456789abcdef";

	

	return buf;
}

// Valid input is assumed, besides base.
unsigned long long strtoull(const char* str, char** endptr, int base) {
	// TODO: Unsupported bases
	if (base > 16 || base < 2) {
		return 0;
	}

	unsigned long long num = 0;
	const char* lowers = "0123456789abcdef";
	const char* uppers = "0123456789ABCDEF";
	const char* loc;

	*endptr = (char*)str;	

	// Skip non-numeric
	while (**endptr == '0') {
		str++;
		(*endptr)++;
	}

	// Skip non-numeric
	if (**endptr == 'x' || **endptr == 'b') {
		str++;
		(*endptr)++;
	}
	
	// Iterate until end of string, or non-hex digit.
	while (**endptr && isxdigit(**endptr)) {
		(*endptr)++;
	}

	(*endptr)--;

	size_t pwr = 1;

	while (*endptr >= str) {
		if (isupper(**endptr)) {
			loc = strchr(uppers, **endptr);
		
			// ERROR: Not in character table.	
			if (!loc) {
				return num;
			}

			num += pwr*(loc-uppers);
		}
		else {
			loc = strchr(lowers, **endptr);
			
			// ERROR: Not in character table.
			if (!loc) {
				return num;
			}

			num += pwr*(loc-lowers);
		}
		pwr *= base;
		(*endptr)--;
	}

	return num;
}


int system(const char* command) {
	return 0;
}
