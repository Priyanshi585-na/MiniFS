#include "disk.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static FILE *disk = NULL;

int open_disk(const char *filename){
    if(disk != NULL) return -1;

    disk = fopen(filename, "rb+");

    if(disk == NULL) return -1;
    return 0;
}



int read_block(uint32_t block_number, void *buffer){
    if(disk == NULL) return -1;
    
    if(block_number >= TOTAL_BLOCKS) return -1;

    fseek(disk, block_number * BLOCK_SIZE, SEEK_SET);

    size_t bytes_read = fread(buffer, BLOCK_SIZE, 1, disk);

    if(bytes_read != 1) return -1;
    return 0;
}




int create_disk(const char* filename){
    FILE *file = fopen(filename, "wb");

    if(file == NULL) return -1;

    fseek(file, BLOCK_SIZE*TOTAL_BLOCKS - 1, SEEK_SET);

    fputc('\0', file);
    fclose(file);

    return 0;
}



int close_disk(){
    if (disk == NULL) return -1;

    fclose(disk);

    disk = NULL;
    return 0;
}



int write_block(uint32_t block_number, const void* buffer){
    if(disk == NULL) return -1;

    if(block_number >= TOTAL_BLOCKS) return -1;

    if(fseek(disk, block_number* BLOCK_SIZE, SEEK_SET) != 0) return -1;

    size_t blocks_written = fwrite(buffer, BLOCK_SIZE, 1, disk);

    if(blocks_written != 1) return -1;

    fflush(disk);
    return 0;
}