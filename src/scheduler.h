#ifndef _SCHEDULER_H
#define _SCHEDULER_H

#include <stddef.h>

// TEB flags
#define TEB_BUSY 1
#define TEB_NAMESIZE 256

// LZLOS Thread Environment Block
typedef struct _lzlos_teb {
	size_t 	image_size;		// Process image size
	void* 	image_base; 		// Process virtual address
	void*	stack;			// Virtual address
	void*	pagetable;		// Page table linear address
	struct _lzlos_teb* prev;
	struct _lzlos_teb* next;
	unsigned char	flags;		// Busy, etc
	unsigned char	priority;	// Higher priority
	size_t		uptime;		// CPU time
	char	name[TEB_NAMESIZE];	// Process name (path), paged
} lzlos_teb;

lzlos_teb* scheduler_task();

void scheduler_switch(lzlos_teb* teb);

#endif
