#include "scheduler.h"
#include "kdefs.h"

// SYS_EXECVE 0x59
int kexecve(const char* path, char* const argv[], char* const envp[]) {
	return 0;
}

// SYS_EXIT 0x60
void kexit(int status) {
	
}


