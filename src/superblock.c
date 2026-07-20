#include <string.h>
#include<stdio.h>
#include "superblock.h"
#include "disk.h"

int save_superblock(const Superblock *sb){
    char block[BLOCK_SIZE] = {0};
    memcpy(block, sb,sizeof(Superblock));

    return write_block(SUPERBLOCK_BLOCK, block);
}

int load_superblock(Superblock *sb){
    char block[BLOCK_SIZE];

    if(read_block(SUPERBLOCK_BLOCK, block) == -1) return -1;

    memcpy(sb, block, sizeof(Superblock));
    return 0;
}

int format_filesystem()
{
    Superblock sb;
    memset(&sb, 0, sizeof(Superblock));

    sb.magic_number = FS_MAGIC;
    sb.version = FS_VERSION;
    sb.block_size = BLOCK_SIZE;
    sb.total_blocks = TOTAL_BLOCKS;
    sb.total_inodes = TOTAL_INODES;

    sb.free_blocks = TOTAL_BLOCKS - DATA_BLOCK_START;
    sb.free_inodes = TOTAL_INODES;

    sb.inode_table_start = INODE_TABLE_START;
    sb.data_block_start = DATA_BLOCK_START;

    int results = save_superblock(&sb);
    return results;
}