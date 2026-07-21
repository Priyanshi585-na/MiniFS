#include <stdio.h>
#include <string.h>

#include "disk.h"
#include "superblock.h"
#include "bitmap.h"
#include "inode.h"
#include "fs.h"

static Superblock superblock;

static int find_inode_by_name(const char *filename)
{
    for (uint32_t i = 0; i < TOTAL_INODES; i++)
    {
        Inode *inode = inode_get(i);

        if (inode && inode->used &&
            strcmp(inode->filename, filename) == 0)
        {
            return i;
        }
    }

    return -1;
}

int fs_create(const char *filename)
{
    if (filename == NULL)
        return -1;

    if (strlen(filename) >= sizeof(((Inode *)0)->filename))
        return -1;

    if (find_inode_by_name(filename) != -1)
        return -1;

    int inode_num = inode_allocate();
    if (inode_num == -1)
        return -1;

    Inode inode;

    if (inode_read(inode_num, &inode) == -1)
        return -1;
    strcpy(inode.filename, filename);
    inode.size = 0;

    memset(inode.direct_blocks, 0, sizeof(inode.direct_blocks));

    if (inode_write(inode_num, &inode) == -1)
        return -1;

    return 0;
}

int fs_format()
{
    if (create_disk(DISK_FILE) == -1)
        return -1;

    if (open_disk(DISK_FILE) == -1)
        return -1;

    if (format_filesystem() == -1)
        return -1;

    if (bitmap_init() == -1)
        return -1;

    if (inode_init() == -1)
        return -1;

    close_disk();

    return 0;
}

int fs_mount()
{
    if (open_disk(DISK_FILE) == -1)
        return -1;

    if (load_superblock(&superblock) == -1)
        return -1;

    if (bitmap_load() == -1)
        return -1;

    if (inode_load() == -1)
        return -1;

    return 0;
}

int fs_unmount()
{
    if (bitmap_save() == -1)
        return -1;

    if (inode_save() == -1)
        return -1;

    if (save_superblock(&superblock) == -1)
        return -1;

    if (close_disk() == -1)
        return -1;

    return 0;
}

int fs_exists(const char *filename)
{
    return find_inode_by_name(filename) != -1;
}

int fs_delete(const char *filename)
{
    if (filename == NULL)
        return -1;

    int inode_num = find_inode_by_name(filename);

    if (inode_num == -1)
        return -1;

    Inode inode;

    if (inode_read(inode_num, &inode) == -1)
        return -1;

    for (int i = 0; i < DIRECT_POINTERS; i++)
    {
        if (inode.direct_blocks[i] != 0)
        {
            bitmap_free(inode.direct_blocks[i]);
        }
    }

    if (inode_free(inode_num) == -1)
        return -1;

    return 0;
}

int fs_write(const char *filename, const void *buffer, uint32_t size)
{
    if (filename == NULL || buffer == NULL)
        return -1;

    if (size > DIRECT_POINTERS * BLOCK_SIZE)
        return -1;

    int inode_num = find_inode_by_name(filename);
    if (inode_num == -1)
        return -1;

    Inode inode;
    if (inode_read(inode_num, &inode) == -1)
        return -1;

    /* Free previously allocated blocks (overwrite semantics) */
    for (uint32_t i = 0; i < DIRECT_POINTERS; i++)
    {
        if (inode.direct_blocks[i] != 0)
        {
            bitmap_free(inode.direct_blocks[i]);
            inode.direct_blocks[i] = 0;
        }
    }

    uint32_t blocks_needed = (size + BLOCK_SIZE - 1) / BLOCK_SIZE;
    const uint8_t *data = (const uint8_t *)buffer;

    for (uint32_t i = 0; i < blocks_needed; i++)
    {
        int block = bitmap_allocate();
        if (block == -1)
            return -1;

        inode.direct_blocks[i] = block;

        /* Temporary block buffer */
        uint8_t block_buffer[BLOCK_SIZE] = {0};

        uint32_t offset = i * BLOCK_SIZE;
        uint32_t bytes_to_copy = size - offset;

        if (bytes_to_copy > BLOCK_SIZE)
            bytes_to_copy = BLOCK_SIZE;

        memcpy(block_buffer, data + offset, bytes_to_copy);

        if (write_block(block, block_buffer) == -1)
        {
            /* Rollback */
            for (uint32_t j = 0; j <= i; j++)
            {
                if (inode.direct_blocks[j] != 0)
                {
                    bitmap_free(inode.direct_blocks[j]);
                    inode.direct_blocks[j] = 0;
                }
            }

            return -1;
        }
    }

    inode.size = size;

    if (inode_write(inode_num, &inode) == -1)
        return -1;

    return 0;
}



int fs_read(const char *filename, void *buffer)
{
    if (filename == NULL || buffer == NULL)
        return -1;

    int inode_num = find_inode_by_name(filename);

    if (inode_num == -1)
        return -1;

    Inode inode;

    if (inode_read(inode_num, &inode) == -1)
        return -1;

    uint32_t blocks_needed =
        (inode.size + BLOCK_SIZE - 1) / BLOCK_SIZE;

    uint8_t *data = (uint8_t *)buffer;

    for (uint32_t i = 0; i < blocks_needed; i++)
    {
        uint8_t block_buffer[BLOCK_SIZE];

        if (read_block(inode.direct_blocks[i], block_buffer) == -1)
            return -1;

        uint32_t offset = i * BLOCK_SIZE;

        uint32_t bytes_to_copy = inode.size - offset;

        if (bytes_to_copy > BLOCK_SIZE)
            bytes_to_copy = BLOCK_SIZE;

        memcpy(data + offset, block_buffer, bytes_to_copy);
    }

    return inode.size;
}


void fs_list()
{
    printf("\n========== MiniFS ==========\n");
    printf("%-32s %10s\n", "Filename", "Size");
    printf("-----------------------------------------------\n");

    for (uint32_t i = 0; i < TOTAL_INODES; i++)
    {
        Inode *inode = inode_get(i);

        if (inode != NULL && inode->used)
        {
            printf("%-32s %10u bytes\n",
                   inode->filename,
                   inode->size);
        }
    }

    printf("-----------------------------------------------\n");
}


