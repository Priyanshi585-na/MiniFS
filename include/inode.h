#ifndef INODE_H
#define INODE_H

#include <stdint.h>

#define MAX_FILENAME_LENGTH 32
#define DIRECT_POINTERS 10

typedef struct{
    uint8_t used;
    char filename[MAX_FILENAME_LENGTH];
    uint32_t size;
    uint32_t direct_blocks[DIRECT_POINTERS];

}Inode;

int inode_init();
int inode_allocate();
int inode_free(uint32_t inode);
int inode_read(uint32_t inode, Inode *out);
int inode_write(uint32_t inode, const Inode *in);
int inode_load();
int inode_save();
Inode *inode_get(uint32_t inode);

#endif
