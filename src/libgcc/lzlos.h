#ifndef _LIBGCC_LZLOS_H
#define _LIBGCC_LZLOS_H

#include <stddef.h> // size_t
#include <errno.h>

#ifdef __cplusplus
extern "C"
#endif

int _start(int argc, char** argv);

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
size_t ulltostr(unsigned long long num, int base, char* dst, size_t size);

typedef int (*putchar_t)(int ch);
typedef size_t (*fwrite_t)(const void* buf, size_t size, size_t count, FILE* f);

// Variadic OS printf with custom stdio functions
int _vprintf(const char* format, va_list ap, putchar_t putchar, fwrite_t fwrite);

#endif
