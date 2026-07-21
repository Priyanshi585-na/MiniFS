# MiniFS Architecture

MiniFS is a simple Unix-inspired filesystem implemented in C. It simulates a block-based storage device using a virtual disk image (`disk.img`) and provides a layered architecture similar to a real operating system.

## Architecture

```
+------------------------+
|   Interactive Shell    |
+------------------------+
            |
            v
+------------------------+
|    Filesystem API      |
|  (create, read, etc.)  |
+------------------------+
            |
            v
+------------------------+
|      Inode Layer       |
+------------------------+
            |
            v
+------------------------+
|    Block Bitmap        |
+------------------------+
            |
            v
+------------------------+
|      Disk Layer        |
+------------------------+
            |
            v
+------------------------+
|       disk.img         |
+------------------------+
```

---

## Components

### Disk Layer

The disk layer provides block-based access to a virtual disk stored in a binary file (`disk.img`).

Functions:

- `create_disk()`
- `open_disk()`
- `close_disk()`
- `read_block()`
- `write_block()`

Every read and write occurs in fixed-size blocks of 4096 bytes.

---

### Superblock

The superblock stores metadata describing the filesystem.

It contains:

- Magic number
- Filesystem version
- Block size
- Total blocks
- Total inodes
- Free blocks
- Free inodes
- Start of inode table
- Start of data region

---

### Block Bitmap

The bitmap tracks which data blocks are allocated.

```
0 = Free
1 = Allocated
```

When a file grows, free blocks are allocated from the bitmap. When a file is deleted, those blocks are returned to the bitmap.

---

### Inode Layer

Each file is represented by one inode.

Each inode stores:

- Filename
- File size
- Allocation status
- Direct block pointers

The current implementation supports 10 direct pointers per file.

---

### Filesystem API

The filesystem layer exposes operations to applications:

- Format filesystem
- Mount / unmount
- Create file
- Delete file
- Write file
- Read file
- List files
- Display file metadata

---

## File Write Flow

```
Application
      |
      v
fs_write()
      |
      v
Find inode
      |
      v
Allocate free blocks
      |
      v
Write blocks to disk
      |
      v
Update inode
```

---

## File Read Flow

```
Application
      |
      v
fs_read()
      |
      v
Read inode
      |
      v
Locate data blocks
      |
      v
Read blocks
      |
      v
Copy into user buffer
```

---

## Current Limitations

MiniFS is designed for learning operating system concepts.

Current limitations include:

- Flat namespace (no directories)
- Maximum of 10 direct data blocks per file
- No indirect block pointers
- No permissions
- No timestamps
- No journaling