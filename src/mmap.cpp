//===============================================
// Kernel Memory Mapper
// 
// void *kalloc(unsigned int size);
// void kfree(void *block);
//
//===============================================

#include "mmap.h"
#include "kdefs.h"
#include "vga.h"

// NULL
#include <stddef.h>

#define HEAP_BASE_VA 	0x200000
#define HEAP_BASE_SIZE	0x200000

void* kalloc(unsigned int size) {
}

void kfree(void* buf) {
}

// Map
void*	kmmap(void* addr, size_t len, int prot, int flags) {
	
	return NULL;
}

int	kmunmap(void* ptr, size_t size) {
	return NULL;
}












