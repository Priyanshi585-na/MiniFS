#ifndef BITMAP_H
#define BITMAP_H

#include <stdint.h>

int bitmap_init();

int bitmap_allocate();
void bitmap_free(uint32_t block_number);
int bitmap_is_allocated(uint32_t block_number);

#endif