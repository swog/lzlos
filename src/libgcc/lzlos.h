#ifndef _LIBGCC_LZLOS_H
#define _LIBGCC_LZLOS_H

#include <stddef.h> // size_t

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

#endif
