#pragma once
#include <stdlib.h>

struct lzlos_teb;

size_t drv_size(lzlos_teb* teb, void **base);
int drv_open(lzlos_teb* teb);
