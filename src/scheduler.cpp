#include "scheduler.h"
#include "kdefs.h"

// SYS_EXECVE 0x59
int kexecve(const char* path, char* const argv[], char* const envp[]) {
	return 0;
}

// SYS_EXIT 0x60
void kexit(int status) {
	
}

// Temporary implementation

lzlos_teb* teb_head = NULL;

// Current task kernel
lzlos_teb* kscheduler_task() {
	return teb_head;
}

// Current task
EXTERN_C lzlos_teb* scheduler_task() {
	return kscheduler_task();
}

lzlos_teb scheduler_teb;

//
// Insert task
//
void scheduler_insert(lzlos_teb* teb) {
	teb->prev = NULL;
	teb->next = teb_head;	

	if (teb_head) {
		teb_head->prev = teb;
	}

	teb_head = teb;
}

//
// Remove task
//
void scheduler_remove(lzlos_teb* teb) {
	if (teb->prev) {
		teb->prev->next = teb->next;
	}

	if (teb->next) {
		teb->next->prev = teb->prev;
	}

	if (teb == teb_head) {
		teb_head = teb->next;
	}
}

void scheduler_main(const void* pagetable) {
	lzlos_teb* teb;

	while (1) {	
		teb = scheduler_task();

		if (teb) {
			scheduler_switch(teb);
		}

		
	}
}
