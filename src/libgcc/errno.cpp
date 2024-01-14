#include <errno.h>

// Thread safe? naahhhh
static int libc_errno = 0;

extern "C" int* __errno_location() {
	return &libc_errno;
}
