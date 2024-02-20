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

#define HEAP_BASE_PHYS 	0x200000
#define HEAP_BASE_SIZE	0x200000


// Heap index type
typedef unsigned int heap_idx;



// 2^(10+2) = 4096
#define HEAP_TABLE_SIZE 10
#define HEAP_SIZE 	64
#define HEAP_INDEX_INVALID ((heap_idx)(-1))
#define HEAP_REVERSE_MOD 509
#define HEAP_REVERSE_SIZE 512

typedef struct {
	heap_idx prev, next;
	void* base;
	//size_t size;
} heap_list;

// Heap `slab` table
static heap_idx heaptable_used[HEAP_TABLE_SIZE];
static heap_idx heaptable_free[HEAP_TABLE_SIZE];

static heap_list 	heaps[HEAP_SIZE];
static heap_idx 	heaps_index = 0;
static char* 		heaps_ptr = (char*)HEAP_BASE_PHYS;

typedef struct {
	void* 	ptr;
	size_t 	index;
} heaps_rev_list;

// Reverse heaps_ptr to heap_idx
// 	This is an iterative hashmap. It iterates over the list to
// find free entries. Each entry does not contain a linked list because
// we cannot allocate more with the heap.
static heaps_rev_list 	heaps_reverse[HEAP_REVERSE_SIZE];

// Basically max(ceil(log2(size)), 2)-2
// Probably faster though
// Addition: heapsize, the size of the base 2 heaptable size
static size_t heaptable_index(size_t size, size_t& heapsize) {
	heapsize = 4;

	if (size <= 4) {
		return 0;
	}

	size_t pos = 2;

	while ((1 << pos) < size) {
		pos++;
		heapsize <<= 1;
	}

	return pos-2;
}

void heap_main() {
	// Sets every index to -1
	for (size_t i = 0; i < HEAP_REVERSE_MOD; i++) {
		heaps_reverse[i].index = -1;
	}
}

// Return index into heaps_reverse
// -1 if error.
static size_t heaps_addrev(void* ptr, heap_idx idx) {
	size_t rev_index = (size_t)ptr % HEAP_REVERSE_MOD;
	size_t i = 0;

	for (i = 0; i < HEAP_REVERSE_MOD && 
		heaps_reverse[rev_index].index != HEAP_INDEX_INVALID; i++) {
		rev_index++;
	}

	if (i == HEAP_REVERSE_MOD) {
		return -1;
	}

	heaps_reverse[rev_index].ptr = ptr;
	heaps_reverse[rev_index].index = idx;	

	return rev_index;
}

static heaps_rev_list* heaps_findrev(const void* ptr) {
	size_t rev_index = (size_t)ptr % HEAP_REVERSE_MOD;
	size_t i = 0;

	for (i = 0; i < HEAP_REVERSE_MOD && 
		heaps_reverse[rev_index].ptr != ptr; i++) {
		rev_index++;
	}

	if (i == HEAP_REVERSE_MOD) {
		return NULL;
	}

	return &heaps_reverse[rev_index];
}

void* kalloc(size_t size) {
	size_t heapsize;
	size_t index = heaptable_index(size, heapsize);

	if (index > HEAP_TABLE_SIZE) {
		return NULL;
	}

	heap_list* heap;

	// Use free heap
	if (heaptable_free[index] != HEAP_INDEX_INVALID) {
		heap = &heaps[heaptable_free[index]];

		// Fix prev
		if (heap->next != HEAP_INDEX_INVALID) {
			heaps[heap->next].prev = HEAP_INDEX_INVALID;
		}

		heaptable_free[index] = heap->next;
	}
	else {
		heap = &heaps[heaps_index];
		heap->prev = HEAP_INDEX_INVALID;
		heap->next = heaptable_used[index];
		heap->base = (void*)heaps_ptr;
		//heap->size = heapsize;

		// Actuate prev if valid heap
		if (heaptable_used[index] != HEAP_INDEX_INVALID) {
			heaps[heaptable_used[heaps_index]].prev = heaps_index;
		}

		heaptable_used[index] = heaps_index;
	
		heaps_index++;
		heaps_ptr += heapsize;
	}

	return heap->base;
}

void kfree(void* buf) {
	heaps_rev_list* rev = heaps_findrev(buf);
	
	if (!rev) {
		return;
	}

	
}

// Map
void*	kmmap(void* addr, size_t len, int prot, int flags) {
	
	return NULL;
}

int	kmunmap(void* ptr, size_t size) {
	return 0;
}












