# MiniFS Disk Layout

## Disk Size
16 MB

## Block Size
4096 bytes

## Total Blocks
4096

## Layout

Block 0   : Superblock
Block 1   : Block Bitmap
Block 2   : Inode Bitmap
Block 3-34: Inode Table
Remaining : Data Blocks