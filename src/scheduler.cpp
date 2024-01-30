#include "scheduler.h"
#include "kdefs.h"

// Temporary implementation

lzlos_teb* teb_head = NULL;

extern "C" lzlos_teb* scheduler_task() {
	return NULL;
}

void scheduler_insert(lzlos_teb* teb) {
	teb->prev = NULL;
	teb->next = teb_head;	

	if (teb_head) {
		teb_head->prev = teb;
	}

	teb_head = teb;
}
