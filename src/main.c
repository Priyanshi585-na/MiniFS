#include <stdio.h>

#include "disk.h"
#include "superblock.h"
#include "bitmap.h"
#include "inode.h"

int main()
{
    create_disk("disk.img");
    open_disk("disk.img");

    if (inode_init() == -1)
    {
        printf("inode_init failed\n");
        return 1;
    }

    bitmap_init();

    int block = bitmap_allocate();

    printf("Allocated: %d\n", block);

    bitmap_load();

    printf("Allocated after load: %d\n", bitmap_is_allocated(block));

    close_disk();
}