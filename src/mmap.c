//===============================================
// Kernel Memory Mapper
// 
// 	Currently two linked lists containing 
// used and free pages. Fragmentation is somewhat
// managed. Speed is a concern for allocation
// and freeing. 
//
// void *kalloc(unsigned int size);
// void kfree(void *block);
//
// 	User allocations should be quicker when
// implemented.
//===============================================

#include "alloc.h"
#include "hmap.h"
#include "mmap.h"
#include "kdefs.h"
#include "vga.h"

typedef struct page_s {
	unsigned int	base;
	unsigned int 	msb;
	// Indices
	unsigned int 	next;
	unsigned int 	prev;
} page_t;

page_t pages_list[512];

// Pointer->page index map
hmap_t			reverse_map;
hmap_bucket_t* 	reverse_buckets[251];

//===============================================
// TODO: Sort these into binary trees ranked by address
// OR we can use hash maps
// Currently, they're just linked lists
//===============================================
unsigned int pages_used[] = {
	-1, // 8
	-1, // 16
	-1, // 32
	-1, // 64
	-1,	// 128
	-1, // 256
	-1,	// 512
	-1, // 1024
	-1, // 2048
	-1, // 4096
	-1, // 8192
};

unsigned int pages_free[] = {
	-1, // 8
	-1, // 16
	-1, // 32
	-1, // 64
	-1,	// 128
	-1, // 256
	-1,	// 512
	-1, // 1024
	-1, // 2048
	-1, // 4096
	-1, // 8192
};

unsigned int pages_open = 0;
unsigned int pages_addr = 0x200000;

//===============================================
// Allocate new page to a base address & size
//===============================================
unsigned int pages_new(unsigned int base, unsigned int msb) {
	unsigned int page = pages_open++;

	pages_list[page].base = base;
	pages_list[page].msb = msb;

	return page;
}

//===============================================
// Insert page to head pointer
//
// Head is an index into the `pages_list`, so is `page`
//===============================================
void pages_insert(unsigned int *head, unsigned int page) {
	pages_list[page].next = *head;
	pages_list[page].prev = -1;
		
	if (*head != -1) {
		pages_list[*head].prev = page;
	}

	*head = page;
}

//===============================================
// Remove a page from the given list `head`
//===============================================
void pages_remove(unsigned int *head, unsigned int page) {
	if (pages_list[page].prev == -1) {
		*head = pages_list[page].next;
		
		if (*head != -1) {
			pages_list[*head].prev = -1;
		}
	}
	else {
		pages_list[pages_list[page].prev].next = pages_list[page].next;
		
		if (pages_list[page].next != -1) {
			pages_list[pages_list[page].next].prev = pages_list[page].prev;
		}
	}

	// No double free
	// Speed up searches
	hmap_remove(&reverse_map, pages_list[page].base);
}

static unsigned int mm_hash(const void* key) {
	return (unsigned int)(unsigned long long)key;
}

static int mm_cmp(const void *a, const void *b) {
	return (int)(unsigned long long)(a-b);
}

// Memory manager init
void mm_init() { 	
	hmap_init(
		&reverse_map, 
		ARRAYSIZE(reverse_buckets), // number of buckets
		0,							// don't allocate buckets 
		kalloc, 
		kfree, 
		mm_hash, mm_cmp
	);
	reverse_map.list = reverse_buckets;
}

#include "vga.h"

static unsigned int kpow(unsigned int base, unsigned int power) {
	while (power) {
		base *= base;
		power--;
	}
	return base;
}

static unsigned int klog(unsigned int num, unsigned int base) {
	unsigned int power = 0;
	while (num) {
		num /= base;
		power++;
	}
	return power;
}

//===============================================
// Allocate a new page
//===============================================
unsigned int pages_use(unsigned int msb) {
	unsigned int page;

	if (pages_free[msb] == -1) {
		page = pages_new(pages_addr, msb);
		pages_addr += kpow(2, msb+3);
	}
	else {
		page = pages_free[msb];
		pages_remove(&pages_free[msb], page);
	}
	
	// Page insertions do not add a reverse mapping
	pages_insert(&pages_used[msb], page);
	
	// Add reverse map
	hmap_insert(&reverse_map, pages_list[page].base, page);

	return page;
}

//===============================================
// Kernel allocator
// O(n) free pages if size > all pages
//===============================================
void *kalloc(unsigned int size) {
	unsigned int msb = klog(size, 2)-2;
	
	unsigned int page = pages_use(msb);

	if (page == -1) {
		return NULL;
	}

	return (void*)pages_list[page].base;
}

//===============================================
// Kernel deallocator
// O(n) if all pages are mapped to one bucket
// O(1) average if hmap uses prime bucket count
//===============================================
void kfree(void *base) {
	unsigned int page = hmap_at(&reverse_map, base);
	
	// TODO: Need to get size index
	pages_remove(&pages_used[pages_list[page].msb], page);

	// Remove reverse map
	hmap_remove(&reverse_map, pages_list[page].base);
}







