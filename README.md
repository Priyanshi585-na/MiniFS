# MiniFS

A simple Unix-inspired filesystem implemented in C.

MiniFS simulates a block-based filesystem using a virtual disk image. It supports file creation, deletion, reading, writing, mounting, and an interactive shell.

This project was built to understand how operating systems manage persistent storage.

## Features

- Virtual block device
- Superblock
- Block bitmap allocation
- Inode table
- Flat file namespace
- File creation & deletion
- Persistent storage
- Interactive shell

## Disk Layout

Block 0
Superblock

↓

Block 1
Block Bitmap

↓

Block 2
Inode Bitmap

↓

Blocks 3-34
Inode Table

↓

Blocks 35-4095
Data Blocks



## Architecture

Application

↓

MiniFS Shell

↓

Filesystem API

↓

Inode Layer

↓

Bitmap Allocator

↓

Disk Layer

↓

disk.img


## Example

MiniFS> create hello.txt

Created.

MiniFS> write hello.txt

Enter text:
Hello MiniFS!

MiniFS> read hello.txt

Hello MiniFS!

MiniFS> ls

hello.txt 14 bytes


## API

fs_format()

fs_mount()

fs_unmount()

fs_create()

fs_delete()

fs_write()

fs_read()

fs_list()

fs_stat()


## Future Work

- Directories
- Indirect block pointers
- Journaling
- File permissions
- Timestamps
- Rename support

