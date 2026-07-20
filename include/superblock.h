#ifndef SUPERBLOCK_H
#define SUPERBLOCK_H

#include <stdint.h>
#include "disk.h"

#define FS_MAGIC 0X4D465331
#define FS_VERSION 1

#define TOTAL_INODES 1024

#define SUPERBLOCK_BLOCK 0
#define BLOCK_BITMAP_BLOCK 1
#define INODE_BITMAP_BLOCK 2

#define INODE_TABLE_START 3
#define INODE_TABLE_BLOCKS 32

#define DATA_BLOCK_START (INODE_TABLE_START + INODE_TABLE_BLOCKS)

typedef struct
{
    uint32_t magic_number;
    uint32_t version;
    uint32_t block_size;
    uint32_t total_blocks;
    uint32_t total_inodes;
    uint32_t free_blocks;
    uint32_t free_inodes;
    uint32_t inode_table_start;
    uint32_t data_block_start;
} Superblock;

int format_filesystem();
int load_superblock(Superblock *sb);
int save_superblock(const Superblock *sb);

#endif