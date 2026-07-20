#include <string.h>

#include "bitmap.h"
#include "disk.h"
#include "superblock.h"
#include "inode.h"

static Inode inode_table[TOTAL_INODES];
static uint8_t inode_bitmap[BLOCK_SIZE];

int inode_init()
{
    memset(inode_bitmap, 0, BLOCK_SIZE);
    memset(inode_table, 0, sizeof(inode_table));

    for (uint32_t i = 0; i < INODE_TABLE_BLOCKS; i++)
    {
        if (write_block(INODE_TABLE_START + i, ((uint8_t *)inode_table) + i * BLOCK_SIZE) == -1)
        {
            return -1;
        }
    }
    return 0;
}

int inode_allocate()
{

    uint32_t inodes_per_block = BLOCK_SIZE / sizeof(Inode);

    for (uint32_t inode = 0; inode < TOTAL_INODES; inode++)
    {
        uint32_t byte = inode / 8;
        uint32_t bit = inode % 8;

        if ((inode_bitmap[byte] & (1 << bit)) == 0)
        {
            inode_bitmap[byte] |= (1 << bit);
            inode_table[inode].used = 1;

            if (write_block(INODE_BITMAP_BLOCK, inode_bitmap) == -1)
                return -1;
            
            uint32_t block = INODE_TABLE_START + (inode/inodes_per_block);
            
            void *buffer = ((uint8_t *) inode_table) + (inode / inodes_per_block)*BLOCK_SIZE;

            if(write_block(block, buffer) == -1) return -1;

            return inode;
        }
    }

    return -1;
}

int inode_free(uint32_t inode)
{
    if(inode >= TOTAL_INODES) return -1;

    uint32_t byte = inode / 8;
    uint32_t bit = inode % 8;

    inode_bitmap[byte] &= ~(1 << bit);
    memset(&inode_table[inode], 0, sizeof(Inode));

    if(write_block(INODE_BITMAP_BLOCK, inode_bitmap) == -1) return -1;

    uint32_t inodes_per_block = BLOCK_SIZE/sizeof(Inode);
    uint32_t block = INODE_TABLE_START + (inode / inodes_per_block);

    void *buffer = ((uint8_t *)inode_table) + (inode/inodes_per_block) * BLOCK_SIZE;

    if(write_block(block, buffer) == -1) return -1;

    return 0;
}

int inode_read(uint32_t inode, Inode *out){
    if(inode >= TOTAL_INODES) return -1;
    memcpy(out, &inode_table[inode], sizeof(Inode));

    return 0;
}

int inode_write(uint32_t inode, const Inode *in)
{
    if(inode >= TOTAL_INODES) return -1;

    memcpy(&inode_table[inode], in, sizeof(Inode));

    uint32_t inodes_per_block = BLOCK_SIZE/sizeof(Inode);
    uint32_t block = INODE_TABLE_START + (inode/inodes_per_block);
    
    void *buffer  = ((uint8_t *)inode_table) + (inode/inodes_per_block) * BLOCK_SIZE;

    if(write_block(block, buffer) == -1) return -1;

    return 0;
}