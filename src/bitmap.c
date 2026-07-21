#include <string.h>

#include "bitmap.h"
#include "disk.h"
#include "superblock.h"

static uint8_t bitmap[BLOCK_SIZE];

int bitmap_init()
{
    memset(bitmap, 0, BLOCK_SIZE);

    for (uint32_t block = 0; block < DATA_BLOCK_START; block++)
    {
        uint32_t byte = block / 8;
        uint32_t bit = block % 8;

        bitmap[byte] |= (1 << bit);
    }

    return write_block(BLOCK_BITMAP_BLOCK, bitmap);
}

int bitmap_allocate()
{
    for (uint32_t block = DATA_BLOCK_START; block < TOTAL_BLOCKS; block++)
    {

        uint32_t byte = block / 8;
        uint32_t bit = block % 8;

        if ((bitmap[byte] & (1 << bit)) == 0)
        {
            bitmap[byte] |= (1 << bit);

            if (write_block(BLOCK_BITMAP_BLOCK, bitmap) == -1)
                return -1;

            return block;
        }
    }
    return -1;
}

void bitmap_free(uint32_t block){
    uint32_t byte = block / 8;
    uint32_t bit = block%8;

    bitmap[byte] &= ~(1 << bit);

    write_block(BLOCK_BITMAP_BLOCK, bitmap);
}

int bitmap_is_allocated(uint32_t block_number)
{
    uint32_t byte = block_number / 8;
    uint32_t bit = block_number % 8;

    return (bitmap[byte] & (1 << bit)) != 0;
}

int bitmap_load()
{
    if(read_block(BLOCK_BITMAP_BLOCK, bitmap) == -1) return -1;
    return 0;
}

int bitmap_save()
{
    if(write_block(BLOCK_BITMAP_BLOCK, bitmap) == -1) return -1;
    return 0;
}