#include <stdio.h>

#include "disk.h"
#include "superblock.h"

int main()
{
    Superblock sb;

    if (create_disk("disk.img") == -1)
    {
        printf("Failed to create disk.\n");
        return 1;
    }

    if (open_disk("disk.img") == -1)
    {
        printf("Failed to open disk.\n");
        return 1;
    }

    if (format_filesystem() == -1)
    {
        printf("Failed to format filesystem.\n");
        close_disk();
        return 1;
    }

    if (load_superblock(&sb) == -1)
    {
        printf("Failed to load superblock.\n");
        close_disk();
        return 1;
    }

    printf("\n===== MiniFS Superblock =====\n");
    printf("Magic Number      : 0x%X\n", sb.magic_number);
    printf("Version           : %u\n", sb.version);
    printf("Block Size        : %u bytes\n", sb.block_size);
    printf("Total Blocks      : %u\n", sb.total_blocks);
    printf("Total Inodes      : %u\n", sb.total_inodes);
    printf("Free Blocks       : %u\n", sb.free_blocks);
    printf("Free Inodes       : %u\n", sb.free_inodes);
    printf("Inode Table Start : %u\n", sb.inode_table_start);
    printf("Data Block Start  : %u\n", sb.data_block_start);

    close_disk();

    return 0;
}