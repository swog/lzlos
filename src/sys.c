#include "kdefs.h"
#include "sys.h"

// Make sure these don't get linked with libgcc
static void ksys_read(kisrcall_t* info) {
}

static void ksys_write(kisrcall_t* info) {
}

static void ksys_open(kisrcall_t* info) {
}

static void ksys_close(kisrcall_t* info) {
}

// Handle syscall IRQs
void ksys_irq(kisrcall_t* info) {
	switch (info->rax) {
		case SYS_READ:
			return ksys_read(info);
		case SYS_WRITE:
			return ksys_write(info);
		case SYS_OPEN:
			return ksys_open(info);
		case SYS_CLOSE:
			return ksys_close(info);

	}

	// Panic
}
