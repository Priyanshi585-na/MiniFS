# Disk Layout

MiniFS stores all filesystem data inside a virtual disk image.

## Layout

```
+---------+-------------------------+
| Block 0 | Superblock              |
+---------+-------------------------+
| Block 1 | Block Bitmap            |
+---------+-------------------------+
| Block 2 | Inode Bitmap            |
+---------+-------------------------+
| 3 - 34  | Inode Table             |
+---------+-------------------------+
|35-4095  | Data Blocks             |
+---------+-------------------------+
```

---

## Constants

```c
BLOCK_SIZE = 4096 bytes

TOTAL_BLOCKS = 4096

TOTAL_INODES = 1024
```

Total virtual disk size:

```
4096 × 4096

≈ 16 MB
```

---

## Inode Structure

Each inode stores:

```
+----------------------+
| Used Flag            |
+----------------------+
| Filename             |
+----------------------+
| File Size            |
+----------------------+
| Direct Block Ptr[10] |
+----------------------+
```

Each file can reference up to ten data blocks.

Maximum file size:

```
10 × 4096

≈ 40 KB
```

---

## Block Allocation

When writing a file:

```
Request write

↓

Bitmap searches for free block

↓

Block allocated

↓

Data written

↓

Inode updated
```

---

## File Deletion

Deleting a file performs:

```
Locate inode

↓

Free all allocated blocks

↓

Clear inode

↓

Update bitmap
```

---

## Example

Suppose the file:

```
hello.txt
```

contains 7000 bytes.

Its inode may contain:

```
Filename : hello.txt

Size : 7000

Direct Blocks

35
36
```

The file occupies:

- Block 35 (4096 bytes)
- Block 36 (remaining 2904 bytes)