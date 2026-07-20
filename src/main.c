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

    int inode = inode_allocate();
    printf("Allocated inode: %d\n", inode);

    Inode temp;

    if (inode_read(inode, &temp) == 0)
    {
        printf("Used: %d\n", temp.used);
    }
    printf("%zu\n", sizeof(Inode));

    close_disk();
}