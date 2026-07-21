#ifndef FS_H
#define FS_H

#define DISK_FILE "disk.img"

int fs_create(const char *filename);
int fs_format();
int fs_mount();
int fs_unmount();
int fs_exists(const char *filename);
int fs_create(const char *filename);
int fs_delete(const char *filename);
int fs_write(const char *filename, const void *buffer, uint32_t size);
int fs_read(const char *filename,void *buffer);
void fs_list();
void fs_stat(const char *filename);

#endif