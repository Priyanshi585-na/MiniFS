#ifndef DISK_H
#define DISK_H

#include <stdint.h>

#define BLOCK_SIZE 4096
#define TOTAL_BLOCKS 4096

int create_disk(const char* filename);
int open_disk(const char* filename);
int close_disk();

int read_block(uint32_t block_number, void *buffer);
int write_block(uint32_t block_number, const void *buffer);

#endif