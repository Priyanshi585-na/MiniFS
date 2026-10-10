#include <stdio.h>
#include <stdint.h>
#include "shell.h"
#include "fs.h"

int main()
{
    FILE *file = fopen(DISK_FILE, "rb");

    if (file == NULL)
    {
        printf("Initializing MiniFS...\n");

        if (fs_format() == -1)
        {
            printf("Failed to format filesystem.\n");
            return 1;
        }
    }
    else
    {
        fclose(file);
    }

    if (fs_mount() == -1)
    {
        printf("Failed to mount filesystem.\n");
        return 1;
    }

    run_shell();

    if (fs_unmount() == -1)
    {
        printf("Failed to unmount filesystem.\n");
        return 1;
    }

    return 0;
}