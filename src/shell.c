#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "shell.h"
#include "fs.h"
#include "disk.h"
#include "superblock.h"
#include "bitmap.h"
#include "inode.h"

#define MAX_COMMAND 100
#define MAX_FILENAME 32

void run_shell()
{
    char command[MAX_COMMAND];
    char filename[MAX_FILENAME];

    printf("=====================================\n");
    printf("        Welcome to MiniFS\n");
    printf("=====================================\n");

    while (1)
    {
        printf("\nMiniFS> ");

        fgets(command, sizeof(command), stdin);

        command[strcspn(command, "\n")] = '\0';

        if (strcmp(command, "exit") == 0)
        {
            printf("Goodbye!\n");
            break;
        }

        else if (strcmp(command, "help") == 0)
        {
            printf("\nAvailable Commands\n");
            printf("------------------\n");
            printf("format\n");
            printf("mount\n");
            printf("unmount\n");
            printf("create <filename>\n");
            printf("write <filename>\n");
            printf("read <filename>\n");
            printf("delete <filename>\n");
            printf("ls\n");
            printf("stat <filename>\n");
            printf("help\n");
            printf("exit\n");
        }

        else if (strcmp(command, "format") == 0)
        {
            if (fs_format() == 0)
                printf("Filesystem formatted.\n");
            else
                printf("Failed.\n");
        }

        else if (strcmp(command, "mount") == 0)
        {
            if (fs_mount() == 0)
                printf("Filesystem mounted.\n");
            else
                printf("Failed.\n");
        }

        else if (strcmp(command, "unmount") == 0)
        {
            if (fs_unmount() == 0)
                printf("Filesystem unmounted.\n");
            else
                printf("Failed.\n");
        }

        else if (sscanf(command, "create %31s", filename) == 1)
        {
            if (fs_create(filename) == 0)
                printf("Created.\n");
            else
                printf("Failed.\n");
        }

        else if (sscanf(command, "delete %31s", filename) == 1)
        {
            if (fs_delete(filename) == 0)
                printf("Deleted.\n");
            else
                printf("Failed.\n");
        }

        else if (strcmp(command, "ls") == 0)
        {
            fs_list();
        }

        else if (sscanf(command, "stat %31s", filename) == 1)
        {
            fs_stat(filename);
        }

        else if (sscanf(command, "write %31s", filename) == 1)
        {
            char buffer[DIRECT_POINTERS * BLOCK_SIZE];

            printf("Enter text:\n");

            fgets(buffer, sizeof(buffer), stdin);

            buffer[strcspn(buffer, "\n")] = '\0';

            if (fs_write(filename,
                         buffer,
                         strlen(buffer) + 1) == 0)
            {
                printf("Written.\n");
            }
            else
            {
                printf("Failed.\n");
            }
        }

        else if (sscanf(command, "read %31s", filename) == 1)
        {
            char buffer[DIRECT_POINTERS * BLOCK_SIZE] = {0};

            if (fs_read(filename, buffer) != -1)
            {
                printf("\n%s\n", buffer);
            }
            else
            {
                printf("Failed.\n");
            }
        }

        else
        {
            printf("Unknown command.\n");
        }
    }
}