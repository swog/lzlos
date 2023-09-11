#include "mmap.h"

typedef struct slabdesc_s {
	int		base;
	int 	size;
	int		prev;
	int		next;
} slabdesc_t;

slabdesc_t *unused_head = (slabdesc_t*)0;
slabdesc_t *used_head = (slabdesc_t*)0;

slabdesc_t *slabs_base = (slabdesc_t*)0x200000;
#define slabs_max (0x1000/sizeof(slabdesc_t))

void mmap_init() {
}

void *kalloc(int size) {

}

void kfree(void *block) {
}
