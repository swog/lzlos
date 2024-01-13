#include "scheduler.h"
#include "kdefs.h"

// Temporary implementation

lzlos_teb current_teb;

extern "C" lzlos_teb* scheduler_task() {
	return &current_teb;
}
