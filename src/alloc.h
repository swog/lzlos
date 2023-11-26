#pragma once

typedef void *allocator_t(unsigned int size);
typedef void deallocator_t(void *base);
