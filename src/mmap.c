#include "mmap.h"
#include "kdefs.h"
#include "vga.h"

typedef struct page_s {
	unsigned int size;
	char data[0];
} page_t;

#define PAGE_USED (1<<31)
#define PAGE_COUNT 256
#define PAGE_BASE 0x200000
#define PAGE_MAX 0x400000
#define PAGE_BIG 0x1000

page_t *pages = (page_t*)PAGE_BASE;

void mm_init() {
	pages->size = 0;
}

static inline void pagefree(page_t *page) {
	page->size &= ~PAGE_USED;
}

static inline int pagesize(page_t *page) {
	return page->size & ~PAGE_USED;
}

static inline int ispageused(page_t *page) {
	return page->size & PAGE_USED;
}

static inline int isbigpage(page_t *page) {
	return pagesize(page) >= PAGE_BIG;
}

void *kalloc(int size) {
	unsigned long long addr = PAGE_BASE;
	int i;
	page_t *page;

	for (i = 0; i < PAGE_COUNT; i++) {
		// It can be equal to the page maximum
		if (addr + size + sizeof(page_t) >= PAGE_MAX) {
			return (void*)0;
		}

		page = (page_t*)addr;		

		// Last page
		if ( !page->size ) {
			break;
		}

		// Page not used, and not last page
		if ( !ispageused(page) ) {
			// If it is a big page, do not size up
			if ( isbigpage(page) ) {
				if ( page->size  == size ) {
					page->size |= PAGE_USED;
					return page->data;
				}
			}
			// If it is greater or equal, size up
			else if ( page->size >= size ) {
				page->size |= PAGE_USED;
				return page->data;
			}
		}

		addr += pagesize(page) + sizeof(page_t);
	}

	// Maximum pages
	if (i >= PAGE_COUNT) {
		return (void*)0;
	}

	page->size = size | PAGE_USED;

	// Set the next page to a break
	if (i + 1 < PAGE_COUNT && addr + size + sizeof(page_t) < PAGE_MAX) {
		addr += size + sizeof(page_t);
		page_t *next_page = (page_t*)addr;
		next_page->size = 0;
	}

	return page->data;
}

void kfree(void *base) {
	page_t *page = (page_t*)((unsigned long long)base - sizeof(page_t));
	pagefree(page);
}
