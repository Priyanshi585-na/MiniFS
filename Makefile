CC = gcc
CFLAGS = -Wall -Wextra -Iinclude

SRC = src/main.c \
      src/shell.c \
      src/fs.c \
      src/disk.c \
      src/superblock.c \
      src/bitmap.c \
      src/inode.c

TARGET = minifs

all:
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET)