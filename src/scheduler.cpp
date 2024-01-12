#include "scheduler.h"

lzlos_teb current_teb;

extern "C" lzlos_teb* scheduler_task() {
	return &current_teb;
}
