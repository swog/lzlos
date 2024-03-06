#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Prime number
#define STDLIB_HEAP_HMAP_MOD 251
#define STDLIB_HEAP_HAMP_SIZE 256

static char 	_stdlib_heap[1024];
static size_t 	_stdlib_heap_pos = 1024;

void* malloc(size_t size) {
	_stdlib_heap_pos -= size;
	return &_stdlib_heap[_stdlib_heap_pos];
}

void free(void* base) {
	// I'm sure this is what Windows does too
}

// Return required-or-used size
// `dst` can be null.
// `size` can be null.
//
// 	This function has a good design scheme, since the
// required size of an ULL is unknown. Calling this function
// twice is memory-economical, and speed-efficient enough for
// this function. 
//
// swog
//
size_t ulltostr(unsigned long long num, int base, char* dst, size_t size) {
	const char* table = "0123456789abcdef";
	unsigned long long digit;	
	size_t i = 1;

	if (dst) {
		dst[size-1] = 0;
	}

	if (!num) {
		if (dst && i < size) {
			dst[size-i-1] = '0';
		}
		i++;
	}
	else {
		// Place base remainder back to front. Then divide by base.
		for (; num != 0; i++) {
			digit = num % base;
			num /= base;

			if (dst && i < size) {
				dst[size-i-1] = table[digit];
			}
		}
	}

	// Pad with zeros to the beginning of the string.
	if (dst) {
		for (; i < size; i++) {
			dst[size-i-1] = '0';
		}
	}

	return i;
}

int system(const char* command) {
	return 0;
}
