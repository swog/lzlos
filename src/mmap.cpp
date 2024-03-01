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

int vmmap1024(void* va, void* ph) {
	char* pt = (char*)get_cr3();
	
}

int vmmap2mb(void* va, void* ph) {
	char* ptr = (char*)get_cr3();
}








