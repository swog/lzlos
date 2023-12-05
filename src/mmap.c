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

#define HEAP_BLOCK_SIZE 0x1000
#define HEAP_BASE_VA 	0x200000
#define HEAP_BASE_SIZE	0x200000
// A block is used
#define HEAP_FLAGS_USED (1<<31)
// All heap flags to determine max size
#define HEAP_FLAGS (HEAP_FLAGS_USED)
#define HEAP_SIZE(pHeap) ((pHeap)->flags_size&~HEAP_FLAGS)
#define HEAP_USED(pHeap) ((pHeap)->flags_size&HEAP_FLAGS_USED)

typedef struct heapentry_s {
	unsigned int flags_size;
	char data[0];
} heapentry_t;

heapentry_t* 	heap_base = (heapentry_t*)HEAP_BASE_VA;
heapentry_t*	heap_end = (heapentry_t*)HEAP_BASE_VA;
int				heap_empty = 0;	
int				heap_init = 1;

heapentry_t* heap_next(heapentry_t* heap) {
	return (heapentry_t*)(((char*)heap)+sizeof(heapentry_t)+HEAP_SIZE(heap));
}

heapentry_t* heap_cast(void* data) {
	return (heapentry_t*)(((char*)data)-sizeof(heapentry_t));
}

heapentry_t* heap_find(unsigned int size) {
	if (heap_empty <= 0) {
		return NULL;
	}

	heapentry_t* hp = heap_base;
	while (hp < heap_end) {
		if (!HEAP_USED(hp) && HEAP_SIZE(hp) >= size) {
			heap_empty--;
			hp->flags_size |= HEAP_FLAGS_USED;
			return hp;
		}
		hp = heap_next(hp);
	}

	return NULL;	
}

void heap_initproc() {
	if (heap_init) {
		heap_base->flags_size = 0;
		heap_init = 0;
	}
}

void* kalloc(unsigned int size) {
	heap_initproc();
	heapentry_t* hp = heap_find(size);

	if (hp) {
		return hp->data;
	}

	hp = heap_end;
	hp->flags_size = HEAP_FLAGS_USED | size;
	heap_end = heap_next(hp);
	return hp->data;
}

void kfree(void* buf) {
	heap_initproc();
	// Add heap to list of unused heaps here.
	heap_empty++;

	heapentry_t* hp = heap_cast(buf);
	hp->flags_size &= ~HEAP_FLAGS_USED;
}














