
#include "kdefs.h"


// VA must be either 1024 or 2mb aligned
int vmmap1024(void* va, void* phys);
int vmmap2mb(void* va, void* phys);
